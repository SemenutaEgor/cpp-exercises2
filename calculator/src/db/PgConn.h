#pragma once

#include <libpq-fe.h>

#include <string>

#include "PgError.h"
#include "PgResult.h"

class PgConn {
 public:
  explicit PgConn(const std::string& conninfo);
  ~PgConn();

  PgConn(const PgConn&) = delete;
  PgConn& operator=(const PgConn&) = delete;

  PgConn(PgConn&& other) noexcept;
  PgConn& operator=(PgConn&& other) noexcept;

  PgResult exec(const std::string& sql);

 private:
  void close() noexcept;

  PGconn* conn_{nullptr};
};