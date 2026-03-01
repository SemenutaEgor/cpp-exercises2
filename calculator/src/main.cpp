#include <pthread.h>
#include <signal.h>

#include <atomic>
#include <csignal>
#include <iostream>
#include <thread>

#include <boost/asio.hpp>
#include "net/TcpServer.h"

#include "core/Runner.h"

int main() {
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

  boost::asio::io_context io;
  TcpServer server(io, 5555, [&](const std::string& line) -> std::string {
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
