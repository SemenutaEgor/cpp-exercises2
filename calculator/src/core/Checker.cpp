#include "Checker.h"

#include <string>

#include "Errors.h"
#include "Request.h"
#include "logging/Logger.h"

void Checker::validate(const Request& request) {
  Logger::instance().debug("Validating request");
  const std::string& op = request.operation();
  const auto& args = request.args();

  if (op == "add" || op == "sub" || op == "mul" || op == "div" || op == "pow") {
    if (args.size() != 2) {
      Logger::instance().error("Validation failed: {}");
      throw ValidationError("operation '" + op +
                            "' requires exactly 2 arguments");
    }

    if (op == "div" && args[1] == 0) {
      Logger::instance().error("Validation failed: {}");
      throw ValidationError("division by zero");
    }

    return;
  }

  if (op == "fact") {
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
  throw ValidationError("unknown operation: " + op);
}
