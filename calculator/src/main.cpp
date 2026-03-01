#include <pthread.h>
#include <signal.h>

#include <atomic>
#include <csignal>
#include <iostream>
#include <thread>

#include "core/Runner.h"

int main() {
  std::atomic_bool stop{false};

  sigset_t set;
  sigemptyset(&set);
  sigaddset(&set, SIGINT);
  sigaddset(&set, SIGTERM);

  pthread_sigmask(SIG_BLOCK, &set, nullptr);

  std::thread signalThread([&](){
   int sig = 0;
   sigwait(&set, &sig);
   std::cout << "signalThread id=" << std::this_thread::get_id() << "\n";
   std::cout << "Signal received: " << sig << "\n";
   stop = true;
  });

  while (!stop) {
   std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  signalThread.join();
  std::cout << "main thread id=" << std::this_thread::get_id() << "\n";
  std::cout << "Graceful shutdown\n";
  return 0;

  // Runner runner;
  // return runner.run(std::cin, std::cout, std::cerr);
}
