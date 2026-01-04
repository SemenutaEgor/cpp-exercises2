#include "Runner.h"

#include <istream>
#include <ostream>

#include "Calculator.h"
#include "Checker.h"
#include "Parser.h"
#include "Printer.h"
#include "Request.h"
#include "Result.h"

int Runner::run(std::istream& in, std::ostream& out, std::ostream& err) {
  Parser parser;
  Checker checker;
  Calculator calculator;
  Printer printer;

  try {
    auto request = parser.parse(in);
    checker.validate(request);
    auto result = calculator.calculate(request);
    printer.print(result, out);
    return 0;
  } catch (...) {
    err << "error\n";
    return 1;
  }
}