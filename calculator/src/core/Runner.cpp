#include "Runner.h"

#include <istream>
#include <ostream>
#include <sstream>

#include "../db/PgConn.h"
#include "../storage/PgStorage.h"
#include "Errors.h"
#include "logging/Logger.h"

Runner::Runner()
    : db_("host=localhost dbname=calculator user=calc_user password=calc_pass"),
      executor_(cache_, db_.storage()) {}

Runner::~Runner() = default;

int Runner::run(std::istream& in, std::ostream& out,
                std::ostream& err) noexcept {
  try {
    Logger::instance().info("Application started");

    executor_.warmup();

    while (processOne(in, out)) {
    }

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

std::string Runner::processLine(const std::string& line) {
  try {
    if (line.find_first_not_of(" \t\r\n") == std::string::npos) {
      return "";
    }

    std::string trimmed = line;
    if (!trimmed.empty() && trimmed.back() == '\r') {
      trimmed.pop_back();
    }

    std::istringstream iss(trimmed);
    std::ostringstream oss;

    auto request = parser_.parse(iss);
    checker_.validate(request);
    auto result = executor_.execute(request, calculator_);
    printer_.print(request, result, oss);

    return oss.str();
  } catch (const AppError& e) {
    return std::string("error: ") + e.what();
  } catch (const std::exception& e) {
    return std::string("internal error: ") + e.what();
  }
}

bool Runner::processOne(std::istream& in, std::ostream& out) {
  std::string line;
  if (!std::getline(in, line)) {
    return false;
  }

  if (line.find_first_not_of(" \t\r\n") == std::string::npos) {
    return true;
  }

  std::istringstream iss(line);

  auto request = parser_.parse(iss);
  checker_.validate(request);
  auto result = executor_.execute(request, calculator_);
  printer_.print(request, result, out);

  return true;
}
