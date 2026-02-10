#pragma once

#include <utility>
#include <vector>

#include "../db/PgConn.h"
#include "OperationKey.h"

class PgStorage {
 public:
  explicit PgStorage(PgConn& conn) : conn_(conn) {}

  std::vector<std::pair<OperationKey, long long>> loadAll();

  void upsert(const OperationKey& key, long long result);

 private:
  PgConn& conn_;
};