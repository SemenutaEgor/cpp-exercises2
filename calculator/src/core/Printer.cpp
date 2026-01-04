#include "Printer.h"

#include <ostream>

#include "Result.h"

void Printer::print(const Result& result, std::ostream& out) {
  out << result.value() << '\n';
}
