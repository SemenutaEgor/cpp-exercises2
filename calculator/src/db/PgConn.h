#pragma once

#include <libpq-fe.h>

#include <vector>
#include <string>
#include <optional>

#include "PgError.h"
#include "PgResult.h"

class PgConn {
 public:
  explicit PgConn(const std::string&);
  ~PgConn();

  PgConn(const PgConn&) = delete;
  PgConn& operator=(const PgConn&) = delete;

  PgConn(PgConn&&) noexcept;
  PgConn& operator=(PgConn&&) noexcept;

  PgResult exec(const std::string&);
  PgResult execParams(const std::string&, const std::vector<std::optional<std::string>>&);

 private:
  void close() noexcept;

  PGconn* conn_{nullptr};
};