#include "Runner.h"

#include <istream>
#include <ostream>
#include <sstream>

#include "Errors.h"
#include "logging/Logger.h"

#include "../storage/OperationKey.h" // временно для проверки

int Runner::run(std::istream& in, std::ostream& out,
                std::ostream& err) noexcept {
  try {
    Logger::instance().info("Application started");
    auto request = parser_.parse(in);

    // TODO для проверки хеширования

    auto key = makeOperationKey(request);

    std::ostringstream oss;
    oss << "OperationKey: op=" << request.operation() << ", args_count=" << key.args.size();

    if (!key.args.empty()) {
     oss << ", first_arg=" << key.args[0];
    }

    if (key.args.size() > 1) {
     oss << ", second_arg=" << key.args[1];
    }

    Logger::instance().info(oss.str());

    //

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
