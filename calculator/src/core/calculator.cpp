#include "calculator.h"

#include <mathlib/mathlib.h>

#include <cstdio>

bool check(const Context& ctx) {
  if (ctx.operation == Operation::Unknown) {
    printf("Error: unknown operation\n");
    return false;
  }

  if (ctx.operation == Operation::Div && ctx.second == 0) {
    printf("Error: division by zero\n");
    return false;
  }

  return true;
}

bool calculate(Context& ctx) {
  int err{};

  switch (ctx.operation) {
    case Operation::Add:
      ctx.result = mathlib::add(ctx.first, ctx.second);
      break;
    case Operation::Sub:
      ctx.result = mathlib::sub(ctx.first, ctx.second);
      break;
    case Operation::Mul:
      ctx.result = mathlib::mul(ctx.first, ctx.second);
      break;
    case Operation::Div:
      ctx.result = mathlib::divide(ctx.first, ctx.second, &err);
      if (err) return false;
      break;
    case Operation::Pow:
      ctx.result = mathlib::pow(ctx.first, ctx.second);
      break;
    case Operation::Fact:
      if (!mathlib::fact(ctx.first, &ctx.result)) {
        printf("Error: overflow\n");
        return false;
      }
      break;
    default:
      return false;
  }

  return true;
}
