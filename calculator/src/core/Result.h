#pragma once

class Result {
 public:
  explicit Result(long long value) noexcept : value_(value) {}

  long long value() const noexcept { return value_; }

 private:
  long long value_;
};
