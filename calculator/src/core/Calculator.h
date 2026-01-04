#pragma once

#include "Result.h"

class Request;

class Calculator {
 public:
  Result calculate(const Request&);
};