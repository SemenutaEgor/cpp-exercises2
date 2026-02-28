#pragma once

#include <string>

#include "../storage/PgStorage.h"
#include "PgConn.h"

class DbContext {
 public:
  explicit DbContext(std::string conninfo);

  PgStorage& storage() noexcept { return storage_; }

 private:
  PgConn conn_;
  PgStorage storage_;
};