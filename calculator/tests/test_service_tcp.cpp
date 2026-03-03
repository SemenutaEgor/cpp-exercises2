#include <gtest/gtest.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

#include <chrono>
#include <stdexcept>
#include <string>

#include "tcp_client.h"

static pid_t start_server(uint16_t port) {
 pid_t pid = ::fork();
 if (pid < 0) {
  throw std::runtime_error("fork failed");
 }

 if (pid == 0) {
  std::string port_str = std::to_string(port);
  execl("./calculator", "./calculator", "--port", port_str.c_str(), (char*)nullptr);
  _exit(127);
 }
 
 return pid;
}

static void stop_server(pid_t pid) {
 if (pid <= 0) return;
 ::kill(pid, SIGTERM);
 int status = 0;
 ::waitpid(pid, &status, 0);
}

TEST(ServiceTcp, MulWorks) {
 const uint16_t port = 5556;
 pid_t pid = -1;

 try {
  pid = start_server(port);

  auto response = send_request("127.0.0.1", port, R"({"operation":"mul","args":[2,3]})", 2000);

  EXPECT_NE(response.find("2 x 3 = 6"), std::string::npos);

  stop_server(pid);
 } catch (...) {
  stop_server(pid);
  throw;
 }
}