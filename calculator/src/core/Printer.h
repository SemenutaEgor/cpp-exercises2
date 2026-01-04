#pragma once

#include <iosfwd>

class Result;

class Printer {
 public:
  void print(const Result&, std::ostream&);
};