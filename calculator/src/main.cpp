#include <pthread.h>
#include <signal.h>

#include <string_view>
#include <string>
#include <atomic>
#include <csignal>
#include <iostream>
#include <thread>

#include <boost/asio.hpp>
#include "net/TcpServer.h"

#include "core/Runner.h"

int main(int argc, char* argv[]) {
  std::atomic_bool stop{false};

  sigset_t set;
  sigemptyset(&set);
  sigaddset(&set, SIGINT);
  sigaddset(&set, SIGTERM);

  if (pthread_sigmask(SIG_BLOCK, &set, nullptr) != 0) {
   std::cerr << "pthread_sigmask failed\n";
   return 1;
  }

  Runner runner;

  bool stdinMode = false;

  for (int i = 1; i < argc; ++i) {
   if (std::string_view(argv[i]) == "--stdin") {
    stdinMode = true;
   }
  }

  if (stdinMode) {
   return runner.run(std::cin, std::cout, std::cerr);
  }

  boost::asio::io_context io;

  unsigned short port = 5555;
  for (int i = 1; i + 1 < argc; ++i) {
   if (std::string_view(argv[i]) == "--port") {
    int p = std::stoi(argv[i + 1]);
    if (p > 0 && p <= 65535) {
     port = static_cast<unsigned short>(p);
    }
   }
  }

  TcpServer server(io, port, [&](const std::string& line) -> std::string {
    auto ans = runner.processLine(line);
    if (ans.empty()) return "\n";
    if (ans.back() != '\n') ans.push_back('\n');
    return ans;
  });
  
  server.start();

  std::thread io_thread([&]{
   io.run();
  });

  std::thread signalThread([&](){
   int sig = 0;
   sigwait(&set, &sig);
   stop = true;

   server.stop();
   io.stop();
  });

  signalThread.join();
  io_thread.join();

  std::cout << "Graceful shutdown\n";
  return 0;
}
