#pragma once

#include <iosfwd>

#include "Request.h"

class Parser {
 public:
  Request parse(std::istream&);
};
