#include <iostream>

#include "core/Runner.h"

int main() {
  Runner runner;
  return runner.run(std::cin, std::cout, std::cerr);
}
