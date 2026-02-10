#include "Runner.h"

#include <istream>
#include <ostream>

#include "Errors.h"
#include "logging/Logger.h"

#include "db/PgConn.h" // test
#include "storage/PgStorage.h" // test

int Runner::run(std::istream& in, std::ostream& out,
                std::ostream& err) noexcept {
  try {
    Logger::instance().info("Application started");

    // test
    PgConn conn("host=localhost dbname=calculator user=calc_user password=calc_pass");

    auto res = conn.execParams("SELECT $1::int + $2::int",
                               {std::string("10"), std::string("3")});
    Logger::instance().info(std::string("sum=") + res.value(0, 0));

    PgStorage storage(conn);
    auto rowsBefore = storage.loadAll();

    Logger::instance().info(std::string("rows before=") + std::to_string(rowsBefore.size()));

    OperationKey key;
    key.op = OpType::ADD;
    key.args = {10, 3};
    normalize(key);

    Logger::instance().info(
        "upsert key: op=" + std::to_string(static_cast<int>(key.op)) +
        " argc=" + std::to_string(key.args.size()) + " a=" +
        std::to_string(key.args[0]) + " b=" + std::to_string(key.args[1]));

    storage.upsert(key, 222);
    Logger::instance().info("upsert done");

    auto rowsAfter = storage.loadAll();
    Logger::instance().info(std::string("rows after=") + std::to_string(rowsAfter.size()));

    auto check = conn.execParams(
        "SELECT result FROM calc_history WHERE op=$1 AND argc=$2 AND a=$3 AND "
        "b=$4",
        {std::to_string(static_cast<int>(key.op)),
         std::to_string(static_cast<int>(key.args.size())),
         std::to_string(key.args[0]), std::to_string(key.args[1])});

    Logger::instance().info(std::string("db_result=") + check.value(0, 0));

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
