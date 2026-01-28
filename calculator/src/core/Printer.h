#pragma once

#include <iosfwd>

class Request;
class Result;

class Printer {
 public:
  void print(const Request&, const Result&, std::ostream&);
};