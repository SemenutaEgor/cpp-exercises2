#include "Calculator.h"

#include <mathlib/mathlib.h>

#include <string>

#include "Errors.h"
#include "Request.h"

Result Calculator::calculate(const Request& request) {
  const std::string& op = request.operation();
  const auto& args = request.args();

  if (op == "add") {
    return Result(mathlib::add(args[0], args[1]));
  }

  if (op == "sub") {
    return Result(mathlib::sub(args[0], args[1]));
  }

  if (op == "mul") {
    return Result(mathlib::mul(args[0], args[1]));
  }

  if (op == "div") {
    int remainder = 0;
    int value = mathlib::divide(args[0], args[1], &remainder);
    return Result(value);
  }

  if (op == "pow") {
    return Result(mathlib::pow(args[0], args[1]));
  }

  if (op == "fact") {
    long long value = 0;
    bool ok = mathlib::fact(args[0], &value);

    if (!ok) {
      throw CalculationError("factorial overflow");
    }

    return Result(value);
  }

  throw CalculationError("unsupported operation: " + op);
}
