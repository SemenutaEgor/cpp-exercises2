#include "Calculator.h"

#include <mathlib/mathlib.h>

#include <string>

#include "Errors.h"
#include "Operations.h"
#include "Request.h"
#include "Result.h"
#include "logging/Logger.h"

Result Calculator::calculate(const Request& request) {
  const std::string& opStr = request.operation();
  const auto op = opFromString(opStr);
  const auto& args = request.args();

  switch (op) {
    case OpType::ADD:
      return Result(mathlib::add(args[0], args[1]));
    case OpType::SUB:
      return Result(mathlib::sub(args[0], args[1]));
    case OpType::MUL:
      return Result(mathlib::mul(args[0], args[1]));
    case OpType::DIV: {
      int remainder = 0;
      int value = mathlib::divide(args[0], args[1], &remainder);
      return Result(value);
    }
    case OpType::POW:
      return Result(mathlib::pow(args[0], args[1]));
    case OpType::FACT: {
      long long value = 0;
      bool ok = mathlib::fact(args[0], &value);

      if (!ok) {
        Logger::instance().error("Calculation failed: factorial overflow");
        throw CalculationError("factorial overflow");
      }

      return Result(value);
    }
    default:
      Logger::instance().error("Calculation failed: unsupported operation");
      throw CalculationError("unsupported operation: " + opStr);
  }
}
