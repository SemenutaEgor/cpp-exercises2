#include "PgConn.h"

PgConn::PgConn(const std::string& conninfo)
    : conn_(PQconnectdb(conninfo.c_str())) {
  if (!conn_) {
    throw PgError("PQconnectdb returned null");
  }
  if (PQstatus(conn_) != CONNECTION_OK) {
    const std::string msg = PQerrorMessage(conn_);
    close();
    throw PgError(msg);
  }
}

PgConn::~PgConn() { close(); }

PgConn::PgConn(PgConn&& other) noexcept : conn_(other.conn_) {
  other.conn_ = nullptr;
}

PgConn& PgConn::operator=(PgConn&& other) noexcept {
  if (this != &other) {
    close();
    conn_ = other.conn_;
    other.conn_ = nullptr;
  }

  return *this;
}

void PgConn::close() noexcept {
  if (conn_) {
    PQfinish(conn_);
    conn_ = nullptr;
  }
}

PgResult PgConn::exec(const std::string& sql) {
  PGresult* res = PQexec(conn_, sql.c_str());
  if (!res) {
    throw PgError(PQerrorMessage(conn_));
  }
  return PgResult(res);
}

PgResult PgConn::execParams(
    const std::string& sql,
    const std::vector<std::optional<std::string>>& params) {
  std::vector<const char*> values;
  values.reserve(params.size());

  for (const auto& p : params) {
    if (p.has_value()) {
      values.push_back(p->c_str());
    } else {
      values.push_back(nullptr);
    }
  }

  PGresult* res =
      PQexecParams(conn_, sql.c_str(), static_cast<int>(values.size()), nullptr,
                   values.data(), nullptr, nullptr, 0);

  if (!res) {
    throw PgError(PQerrorMessage(conn_));
  }

  return PgResult(res);
}
