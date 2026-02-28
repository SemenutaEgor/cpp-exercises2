#include "PgStorage.h"

#include <optional>
#include <string>

#include "../logging/Logger.h"

static auto to_ll(const char* s) -> long long { return std::stoll(s); }

static auto to_i(const char* s) -> int { return std::stoi(s); }

std::vector<std::pair<OperationKey, long long>> PgStorage::loadAll() {
  Logger::instance().info("Loading history from DB");
  auto res = conn_.exec("SELECT op, argc, a, b, result FROM calc_history");

  if (res.status() != PGRES_TUPLES_OK) {
    throw PgError(std::string("loadAll failed: ") + res.error_message());
  }

  std::vector<std::pair<OperationKey, long long>> out;
  out.reserve(res.rows());

  for (int r = 0; r < res.rows(); ++r) {
    OperationKey key;
    key.op = static_cast<OpType>(to_i(res.value(r, 0)));
    const int argc = to_i(res.value(r, 1));
    key.args.clear();
    key.args.reserve(argc);

    key.args.push_back(to_ll(res.value(r, 2)));

    if (argc == 2) {
      if (res.is_null(r, 3)) {
        throw PgError("DB invariant violated: argc=2 but b is NULL");
      }
      key.args.push_back(to_ll(res.value(r, 3)));
    }

    normalize(key);

    const long long result = to_ll(res.value(r, 4));
    out.emplace_back(std::move(key), result);
  }

  return out;
}

void PgStorage::upsert(const OperationKey& key, long long result) {
  const int argc = static_cast<int>(key.args.size());

  std::vector<std::optional<std::string>> params;
  params.reserve(5);

  params.emplace_back(std::to_string(static_cast<int>(key.op)));
  params.emplace_back(std::to_string(argc));
  params.emplace_back(std::to_string(key.args[0]));

  if (argc == 2) {
    params.emplace_back(std::to_string(key.args[1]));
  } else {
    params.emplace_back(std::nullopt);
  }

  params.emplace_back(std::to_string(result));

  auto res = conn_.execParams(
      "INSERT INTO calc_history(op, argc, a, b, result) "
      "VALUES ($1,$2,$3,$4,$5) "
      "ON CONFLICT (op, argc, a, b) "
      "DO UPDATE SET result = EXCLUDED.result",
      params);

  if (res.status() != PGRES_COMMAND_OK) {
    throw PgError(std::string("upsert failed: ") + res.error_message());
  }
}