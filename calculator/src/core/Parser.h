#pragma once

#include <iosfwd>

class Request;

class Parser {
 public:
  Request parse(std::istream&);
};
