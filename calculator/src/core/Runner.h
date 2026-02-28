#pragma once

#include <iosfwd>
#include <string>

#include "../db/DbContext.h"
#include "../storage/Cache.h"
#include "../storage/CachedExecutor.h"
#include "Calculator.h"
#include "Checker.h"
#include "Parser.h"
#include "Printer.h"

class PgConn;
class PgStorage;

class Runner {
 public:
  Runner();
  ~Runner();
  int run(std::istream&, std::ostream&, std::ostream&) noexcept;

 private:
  bool processOne(std::istream& in, std::ostream& out);

 private:
  Parser parser_;
  Checker checker_;
  Calculator calculator_;
  Printer printer_;
  Cache cache_;

  DbContext db_;
  CachedExecutor executor_;
};
