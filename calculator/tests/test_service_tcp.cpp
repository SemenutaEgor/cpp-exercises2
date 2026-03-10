#include <gtest/gtest.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

#include <atomic>
#include <chrono>
#include <latch>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include "tcp_client.h"

namespace {
struct StartBarrier {
  explicit StartBarrier(int total) : total_(total) {}

  void arrive_and_wait() {
    std::unique_lock<std::mutex> lk(m_);
    ++ready_;
    if (ready_ == total_) {
      cv_ready_.notify_one();
    }
    cv_go_.wait(lk, [&] { return go_; });
  }

  void wait_all_ready() {
    std::unique_lock<std::mutex> lk(m_);
    cv_ready_.wait(lk, [&] { return ready_ == total_; });
  }

  void release_all() {
    std::lock_guard<std::mutex> lk(m_);
    go_ = true;
    cv_go_.notify_all();
  }

 private:
  int total_;
  int ready_ = 0;
  bool go_ = false;
  std::mutex m_;
  std::condition_variable cv_ready_;
  std::condition_variable cv_go_;
};

void wait_server_ready(uint16_t port, int timeout_ms) {
  using clock = std::chrono::steady_clock;
  auto deadline = clock::now() + std::chrono::milliseconds(timeout_ms);

  while (clock::now() < deadline) {
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
      throw std::runtime_error("socket failed");
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    ::inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    int rc = ::connect(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
    ::close(fd);

    if (rc == 0) {
      return;
    }

    ::usleep(2000);
  }

  throw std::runtime_error("server not ready within timeout");
}

#ifndef CALCULATOR_BINARY_PATH
#error "CALCULATOR_BINARY_PATH is not defined"
#endif

static pid_t start_server(uint16_t port) {
  pid_t pid = ::fork();
  if (pid < 0) {
    throw std::runtime_error("fork failed");
  }

  if (pid == 0) {
    std::string port_str = std::to_string(port);
    ::execl(CALCULATOR_BINARY_PATH, CALCULATOR_BINARY_PATH, "--port",
            port_str.c_str(), (char*)nullptr);
    perror("execl failed");
    _exit(127);
  }

  return pid;
}

static void stop_server(pid_t pid) {
  if (pid <= 0) return;

  ::kill(pid, SIGTERM);

  int status = 0;
  for (int i = 0; i < 200; ++i) {
    pid_t rc = ::waitpid(pid, &status, WNOHANG);
    if (rc == pid) return;
    ::usleep(10'000);
  }

  ::kill(pid, SIGKILL);
  ::waitpid(pid, &status, 0);
}
}  // namespace

TEST(ServiceTcp, MulWorks) {
  const uint16_t port = 5556;
  pid_t pid = -1;

  try {
    pid = start_server(port);

    auto response = send_request("127.0.0.1", port,
                                 R"({"operation":"mul","args":[2,3]})", 2000);

    EXPECT_NE(response.find("2 x 3 = 6"), std::string::npos);

    stop_server(pid);
  } catch (...) {
    stop_server(pid);
    throw;
  }
}

TEST(ServiceTcp, ManyClientsSameRequest) {
  const uint16_t port = 5556;
  pid_t pid = -1;

  try {
    pid = start_server(port);
    wait_server_ready(port, 8000);

    constexpr int M = 200;
    std::vector<std::string> results(M);
    std::vector<std::thread> threads;
    threads.reserve(M);

    StartBarrier barrier(M);

    for (int i = 0; i < M; ++i) {
      threads.emplace_back([&, i] {
        barrier.arrive_and_wait();
        results[i] = send_request("127.0.0.1", port,
                                  R"({"operation":"mul","args":[2,3]})", 2000);
      });
    }

    barrier.wait_all_ready();
    barrier.release_all();

    for (auto& t : threads) t.join();

    for (const auto& r : results) {
      EXPECT_NE(r.find("2 x 3 = 6"), std::string::npos);
    }

    stop_server(pid);
  } catch (...) {
    stop_server(pid);
    throw;
  }
}

TEST(ServiceTcp, ManyClientsUniqueRequests) {
  const uint16_t port = 5557;
  pid_t pid = -1;

  try {
    pid = start_server(port);
    wait_server_ready(port, 2000);

    constexpr int M = 16;

    std::vector<std::string> results(M);
    std::vector<std::thread> threads;
    threads.reserve(M);

    StartBarrier barrier(M);

    for (int i = 0; i < M; ++i) {
      threads.emplace_back([&, i] {
        barrier.arrive_and_wait();

        const int a = i + 1;
        const int b = i + 2;

        const std::string request =
            std::string("{\"operation\":\"mul\",\"args\":[") +
            std::to_string(a) + "," + std::to_string(b) + "]}";

        results[i] = send_request("127.0.0.1", port, request, 2000);
      });
    }

    barrier.wait_all_ready();
    barrier.release_all();

    for (auto& t : threads) {
      t.join();
    }

    for (int i = 0; i < M; ++i) {
      const int a = i + 1;
      const int b = i + 2;

      const std::string expected = std::to_string(a) + " x " +
                                   std::to_string(b) + " = " +
                                   std::to_string(a * b);

      EXPECT_NE(results[i].find(expected), std::string::npos)
          << "client " << i << ", response: " << results[i];
    }

    stop_server(pid);
  } catch (...) {
    stop_server(pid);
    throw;
  }
}