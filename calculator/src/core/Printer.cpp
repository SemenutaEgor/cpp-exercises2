#include "Printer.h"

#include <ostream>

#include "Operations.h"
#include "Request.h"
#include "Result.h"

void Printer::print(const Request& request, const Result& result,
                    std::ostream& out) {
  const auto op = opFromString(request.operation());

  switch (op) {
    case OpType::ADD:
      out << request.args()[0] << " + " << request.args()[1] << " = "
          << result.value() << '\n';
      return;
    case OpType::SUB:
      out << request.args()[0] << " - " << request.args()[1] << " = "
          << result.value() << '\n';
      return;
    case OpType::MUL:
      out << request.args()[0] << " x " << request.args()[1] << " = "
          << result.value() << '\n';
      return;
    case OpType::DIV:
      out << request.args()[0] << " / " << request.args()[1] << " = "
          << result.value() << '\n';
      return;

    case OpType::POW:
      out << request.args()[0] << " ^ " << request.args()[1] << " = "
          << result.value() << '\n';
      return;
    case OpType::FACT:
      out << request.args()[0] << " ! = " << result.value() << '\n';
      return;
    default:
      return;
  }
}
