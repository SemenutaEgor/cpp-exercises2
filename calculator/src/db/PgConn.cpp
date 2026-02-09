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