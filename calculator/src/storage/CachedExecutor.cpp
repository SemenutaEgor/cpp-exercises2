#include "CachedExecutor.h"

#include "../core/Calculator.h"
#include "../core/Request.h"
#include "../core/Result.h"
#include "../logging/Logger.h"
#include "OperationKey.h"

Result CachedExecutor::execute(const Request& request, Calculator& calculator) {
  const auto key = makeOperationKey(request);
  auto cached = cache_.get(key);

  if (cached) {
    Logger::instance().info("CACHE HIT");
    return Result(*cached);
  }

  Logger::instance().info("CACHE MISS");
  auto result = calculator.calculate(request);
  cache_.put(key, result.value());
  return result;
}