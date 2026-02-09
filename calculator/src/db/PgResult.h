#pragma once

#include <libpq-fe.h>

class PgResult {
 public:
  explicit PgResult(PGresult* res) noexcept : res_(res) {};
  ~PgResult() { clear(); }

  PgResult(const PgResult&) = delete;
  PgResult& operator=(const PgResult&) = delete;

  PgResult(PgResult&& other) noexcept : res_(other.res_) {
    other.res_ = nullptr;
  }
  PgResult& operator=(PgResult&& other) noexcept {
    if (this != &other) {
      clear();
      res_ = other.res_;
      other.res_ = nullptr;
    }
    return *this;
  }

  ExecStatusType status() const noexcept { return PQresultStatus(res_); }
  int rows() const noexcept { return PQntuples(res_); }
  int cols() const noexcept { return PQnfields(res_); }

  const char* value(int row, int col) const {
    return PQgetvalue(res_, row, col);
  }
  bool is_null(int row, int col) const noexcept {
    return PQgetisnull(res_, row, col) != 0;
  }

  const char* error_message() const noexcept {
    return PQresultErrorMessage(res_);
  }

 private:
  void clear() noexcept {
    if (res_) {
      PQclear(res_);
      res_ = nullptr;
    }
  }

  PGresult* res_{nullptr};
};