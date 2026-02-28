#include "CachedExecutor.h"

#include "../core/Calculator.h"
#include "../core/Request.h"
#include "../core/Result.h"
#include "../logging/Logger.h"
#include "OperationKey.h"
#include "PgStorage.h"

void CachedExecutor::warmup() {
  Logger::instance().info("Warming up cache from DB");

  auto rows = storage_.loadAll();

  for (const auto& [key, value] : rows) {
    cache_.put(key, value);
  }

  Logger::instance().info("Cache warmed up");
}

Result CachedExecutor::execute(const Request& request, Calculator& calculator) {
  const auto key = makeOperationKey(request);

  if (auto cached = cache_.get(key)) {
    Logger::instance().info("CACHE HIT");
    return Result(*cached);
  }

  Logger::instance().info("CACHE MISS");
  auto result = calculator.calculate(request);
  cache_.put(key, result.value());
  storage_.upsert(key, result.value());
  return result;
}