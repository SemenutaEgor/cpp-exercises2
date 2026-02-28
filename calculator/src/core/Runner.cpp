#include "Runner.h"

#include <istream>
#include <ostream>

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

    while (true) {
      in >> std::ws;
      if (in.peek() == std::char_traits<char>::eof()) {
        break;
      }

      auto request = parser_.parse(in);
      checker_.validate(request);
      auto result = executor_.execute(request, calculator_);
      printer_.print(request, result, out);
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
