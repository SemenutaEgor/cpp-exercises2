#include "Runner.h"

#include <istream>
#include <ostream>
#include <string>

#include "../db/PgConn.h"
#include "../storage/CachedExecutor.h"
#include "../storage/PgStorage.h"
#include "Errors.h"
#include "logging/Logger.h"

Runner::Runner() {
  const std::string conninfo =
      "host=localhost dbname=calculator user=calc_user password=calc_pass";
  
  conn_ = std::make_unique<PgConn>(conninfo);
  storage_ = std::make_unique<PgStorage>(*conn_);
  executor_ = std::make_unique<CachedExecutor>(cache_, *storage_);
}

Runner::~Runner() = default;

int Runner::run(std::istream& in, std::ostream& out,
                std::ostream& err) noexcept {
  try {
    Logger::instance().info("Application started");

    executor_->warmup();

    while (true) {
      in >> std::ws;
      if (in.peek() == std::char_traits<char>::eof()) {
        break;
      }

      auto request = parser_.parse(in);
      checker_.validate(request);
      auto result = executor_->execute(request, calculator_);
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
