#include "Runner.h"

#include <istream>
#include <ostream>

#include "Errors.h"
#include "logging/Logger.h"

int Runner::run(std::istream& in, std::ostream& out,
                std::ostream& err) noexcept {
  try {
    Logger::instance().info("Application started");
    auto request = parser_.parse(in);
    checker_.validate(request);
    auto result = calculator_.calculate(request);
    printer_.print(request, result, out);
    Logger::instance().info("Application finished successfully");
    return 0;
  } catch (const AppError& e) {
    err << e.what() << '\n';
    Logger::instance().error(e.what());
    return 1;
  } catch (const std::exception& e) {
    err << "internal error: " << e.what() << '\n';
    Logger::instance().error(e.what());
    return 2;
  }
}
