#pragma once

enum class Operation { Add, Sub, Mul, Div, Pow, Fact, Unknown };

struct Context {
  int first{};
  int second{};
  Operation operation{Operation::Unknown};
  long long result{};
};
