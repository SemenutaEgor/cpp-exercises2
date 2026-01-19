#include "Checker.h"

#include <string>

#include "Errors.h"
#include "Operations.h"
#include "Request.h"
#include "logging/Logger.h"

void Checker::validate(const Request& request) {
  Logger::instance().debug("Validating request");
  const std::string& opStr = request.operation();
  const auto op = opFromString(opStr);
  const auto& args = request.args();

  if (op == OpType::ADD || op == OpType::SUB || op == OpType::MUL ||
      op == OpType::DIV || op == OpType::POW) {
    if (args.size() != 2) {
      Logger::instance().error("Validation failed: {}");
      throw ValidationError("operation '" + opStr +
                            "' requires exactly 2 arguments");
    }

    if (op == OpType::DIV && args[1] == 0) {
      Logger::instance().error("Validation failed: {}");
      throw ValidationError("division by zero");
    }

    return;
  }

  if (op == OpType::FACT) {
    if (args.size() != 1) {
      Logger::instance().error("Validation failed: {}");
      throw ValidationError("operation 'fact' requires exactly 1 argument");
    }

    if (args[0] < 0) {
      Logger::instance().error("Validation failed: {}");
      throw ValidationError("factorial argument must be non-negative");
    }

    return;
  }

  Logger::instance().error("Validation failed: {}");
  throw ValidationError("unknown operation: " + opStr);
}
