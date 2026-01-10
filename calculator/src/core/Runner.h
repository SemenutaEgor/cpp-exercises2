#pragma once

#include <iosfwd>

class Runner {
 public:
  int run(std::istream&, std::ostream&, std::ostream&);
};
