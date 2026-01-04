#include "Runner.h"

#include <istream>
#include <ostream>

#include "Calculator.h"
#include "Checker.h"
#include "Parser.h"
#include "Printer.h"
#include "Request.h"
#include "Result.h"
#include "Errors.h"

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
  } catch (const AppError& e) {
    err << e.what() << '\n';
    return 1;
  } catch (const std::exception& e) {
    err << "internal error: " << e.what() << '\n';
    return 2;
  }
}
