#include <gtest/gtest.h>

#include "core/Checker.h"
#include "core/Errors.h"
#include "core/Request.h"

TEST(CheckerTest, ValidAdd) {
  Request req("add", {1, 2});
  Checker checker;

  EXPECT_NO_THROW(checker.validate(req));
}

TEST(CheckerTest, WrongArgsCountAdd) {
  Request req("add", {1});
  Checker checker;

  EXPECT_THROW(checker.validate(req), ValidationError);
}

TEST(CheckerTest, ValidSub) {
  Request req("sub", {1, 2});
  Checker checker;

  EXPECT_NO_THROW(checker.validate(req));
}

TEST(CheckerTest, WrongArgsCountSub) {
  Request req("sub", {1});
  Checker checker;

  EXPECT_THROW(checker.validate(req), ValidationError);
}

TEST(CheckerTest, ValidMul) {
  Request req("mul", {1, 2});
  Checker checker;

  EXPECT_NO_THROW(checker.validate(req));
}

TEST(CheckerTest, WrongArgsCountMul) {
  Request req("mul", {1});
  Checker checker;

  EXPECT_THROW(checker.validate(req), ValidationError);
}

TEST(CheckerTest, ValidDiv) {
  Request req("div", {1, 2});
  Checker checker;

  EXPECT_NO_THROW(checker.validate(req));
}

TEST(CheckerTest, DivisionByZero) {
  Request req("div", {10, 0});
  Checker checker;

  EXPECT_THROW(checker.validate(req), ValidationError);
}

TEST(CheckerTest, WrongArgsCountDiv) {
  Request req("div", {1});
  Checker checker;

  EXPECT_THROW(checker.validate(req), ValidationError);
}

TEST(CheckerTest, DivisionWithNegativeNumbers) {
  Request req("div", {-10, 2});
  Checker checker;

  EXPECT_NO_THROW(checker.validate(req));
}

TEST(CheckerTest, ValidPow) {
  Request req("pow", {1, 2});
  Checker checker;

  EXPECT_NO_THROW(checker.validate(req));
}

TEST(CheckerTest, WrongArgsCountPow) {
  Request req("pow", {1});
  Checker checker;

  EXPECT_THROW(checker.validate(req), ValidationError);
}

TEST(CheckerTest, ValidFact) {
  Request req("fact", {1});
  Checker checker;

  EXPECT_NO_THROW(checker.validate(req));
}

TEST(CheckerTest, WrongArgsCountFact) {
  Request req("fact", {1, 2});
  Checker checker;

  EXPECT_THROW(checker.validate(req), ValidationError);
}

TEST(CheckerTest, FactorialNegativeArgument) {
  Request req("fact", {-1});
  Checker checker;

  EXPECT_THROW(checker.validate(req), ValidationError);
}

TEST(CheckerTest, FactorialZeroIsValid) {
  Request req("fact", {0});
  Checker checker;

  EXPECT_NO_THROW(checker.validate(req));
}

TEST(CheckerTest, UnknownOperation) {
  Request req("xxx", {1, 2});
  Checker checker;

  EXPECT_THROW(checker.validate(req), ValidationError);
}
