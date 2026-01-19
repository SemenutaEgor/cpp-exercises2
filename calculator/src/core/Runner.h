#pragma once

#include <iosfwd>

#include "Calculator.h"
#include "Checker.h"
#include "Parser.h"
#include "Printer.h"

class Calculator;
class Checker;
class Parser;
class Printer;

class Runner {
 public:
  int run(std::istream&, std::ostream&, std::ostream&) noexcept;

 private:
  Parser parser_;
  Checker checker_;
  Calculator calculator_;
  Printer printer_;
};
