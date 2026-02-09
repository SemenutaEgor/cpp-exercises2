#include "Runner.h"

#include <istream>
#include <ostream>

#include "Errors.h"
#include "logging/Logger.h"

#include "db/PgConn.h" // test

int Runner::run(std::istream& in, std::ostream& out,
                std::ostream& err) noexcept {
  try {
    Logger::instance().info("Application started");

    // test
    PgConn conn("host=localhost dbname=calculator user=calc_user password=calc_pass");
    auto res = conn.exec("SELECT 1");
    Logger::instance().info(std::string("PG status: ") + PQresStatus(res.status()));

    // auto request = parser_.parse(in);
    // checker_.validate(request);

    // auto result1 = executor_.execute(request, calculator_);
    // printer_.print(request, result1, out);
    
    // auto result2 = executor_.execute(request, calculator_);
    // printer_.print(request, result2, out);

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
