#pragma once

#include "Cache.h"

class Calculator;
class Request;
class Result;

class CachedExecutor {
 public:
  explicit CachedExecutor(Cache& cache) : cache_(cache) {}

  Result execute(const Request&, Calculator&);

 private:
  Cache& cache_;
};