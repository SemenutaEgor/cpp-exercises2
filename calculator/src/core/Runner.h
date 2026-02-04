#pragma once

#include <iosfwd>

#include "../storage/Cache.h"
#include "../storage/CachedExecutor.h"
#include "Calculator.h"
#include "Checker.h"
#include "Parser.h"
#include "Printer.h"

class Runner {
 public:
  int run(std::istream&, std::ostream&, std::ostream&) noexcept;

 private:
  Parser parser_;
  Checker checker_;
  Calculator calculator_;
  Printer printer_;
  Cache cache_;
  CachedExecutor executor_{cache_};
};
