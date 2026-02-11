#pragma once

#include <iosfwd>
#include <memory>

#include "../storage/Cache.h"
#include "Calculator.h"
#include "Checker.h"
#include "Parser.h"
#include "Printer.h"

class PgConn;
class PgStorage;
class CachedExecutor;

class Runner {
 public:
  Runner();
  ~Runner();
  int run(std::istream&, std::ostream&, std::ostream&) noexcept;

 private:
  Parser parser_;
  Checker checker_;
  Calculator calculator_;
  Printer printer_;
  Cache cache_;
  std::unique_ptr<PgConn> conn_;
  std::unique_ptr<PgStorage> storage_;
  std::unique_ptr<CachedExecutor> executor_;
};
