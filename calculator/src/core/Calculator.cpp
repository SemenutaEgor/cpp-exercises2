#include "Calculator.h"

#include "Errors.h"
#include "Result.h"

Result Calculator::calculate(const Request&) {
  throw CalculationError("calculation not implemented");
}
