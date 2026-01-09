#include <gtest/gtest.h>

#include "core/Calculator.h"
#include "core/Errors.h"
#include "core/Request.h"

TEST(CalculatorTest, Add) {
  Request req("add", {2, 3});
  Calculator calc;

  Result res = calc.calculate(req);
  EXPECT_EQ(res.value(), 5);
}

TEST(CalculatorTest, Sub) {
  Request req("sub", {2, 3});
  Calculator calc;

  Result res = calc.calculate(req);
  EXPECT_EQ(res.value(), -1);
}

TEST(CalculatorTest, Mul) {
  Request req("mul", {2, 3});
  Calculator calc;

  Result res = calc.calculate(req);
  EXPECT_EQ(res.value(), 6);
}

TEST(CalculatorTest, Div) {
  Request req("div", {6, 3});
  Calculator calc;

  Result res = calc.calculate(req);
  EXPECT_EQ(res.value(), 2);
}

TEST(CalculatorTest, Pow) {
  Request req("pow", {6, 3});
  Calculator calc;

  Result res = calc.calculate(req);
  EXPECT_EQ(res.value(), 216);
}

TEST(CalculatorTest, Factorial) {
  Request req("fact", {5});
  Calculator calc;

  Result res = calc.calculate(req);
  EXPECT_EQ(res.value(), 120);
}

TEST(CalculatorTest, FactorialOverflow) {
  Request req("fact", {50});
  Calculator calc;

  EXPECT_THROW(calc.calculate(req), CalculationError);
}

TEST(CalculatorTest, UnknownOperation) {
  Request req("unknown", {1, 2});
  Calculator calc;

  EXPECT_THROW(calc.calculate(req), CalculationError);
}
