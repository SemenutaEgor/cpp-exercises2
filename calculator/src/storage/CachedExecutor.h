#pragma once

#include "Cache.h"

class Calculator;
class Request;
class Result;
class PgStorage;

class CachedExecutor {
 public:
  explicit CachedExecutor(Cache& cache, PgStorage& storage)
      : cache_(cache), storage_(storage) {}

  void warmup();
  Result execute(const Request&, Calculator&);

 private:
  Cache& cache_;
  PgStorage& storage_;
};