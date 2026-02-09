#pragma once

#include <stdexcept>
#include <string>

class PgError : public std::runtime_error {
 public:
  explicit PgError(const std::string& msg) : std::runtime_error(msg) {}
};