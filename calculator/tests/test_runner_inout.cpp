#include <gtest/gtest.h>

#include <sstream>

#include "core/Runner.h"

TEST(InOutTest, AddSuccess) {
  std::istringstream in(R"({"operation":"add","args":[1,2]})");
  std::ostringstream out, err;

  Runner r;
  int code = r.run(in, out, err);

  EXPECT_EQ(code, 0);
  EXPECT_EQ(err.str(), "");
  EXPECT_EQ(out.str(), "3\n");
}

TEST(InOutTest, FactorialSuccess) {
  std::istringstream in(R"({"operation":"fact","args":[5]})");
  std::ostringstream out, err;

  Runner r;
  int code = r.run(in, out, err);

  EXPECT_EQ(code, 0);
  EXPECT_EQ(out.str(), "120\n");
}

TEST(InOutTest, ParseErrorInvalidJson) {
  std::istringstream in(R"({"operation":"add","args":[1,2])");
  std::ostringstream out, err;

  Runner r;
  int code = r.run(in, out, err);

  EXPECT_EQ(code, 1);
  EXPECT_EQ(out.str(), "");
  EXPECT_FALSE(err.str().empty());
}

TEST(InOutTest, ParseErrorWrongTypes) {
  std::istringstream in(R"({"operation":123,"args":42})");
  std::ostringstream out, err;

  Runner r;
  int code = r.run(in, out, err);

  EXPECT_EQ(code, 1);
}

TEST(InOutTest, ValidationErrorWrongArgsCount) {
  std::istringstream in(R"({"operation":"add","args":[1]})");
  std::ostringstream out, err;

  Runner r;
  int code = r.run(in, out, err);

  EXPECT_EQ(code, 1);
}

TEST(InOutTest, ValidationErrorDivisionByZero) {
  std::istringstream in(R"({"operation":"div","args":[10,0]})");
  std::ostringstream out, err;

  Runner r;
  int code = r.run(in, out, err);

  EXPECT_EQ(code, 1);
}

TEST(InOutTest, ValidationErrorNegativeFactorial) {
  std::istringstream in(R"({"operation":"fact","args":[-1]})");
  std::ostringstream out, err;

  Runner r;
  int code = r.run(in, out, err);

  EXPECT_EQ(code, 1);
}

TEST(InOutTest, CalculationErrorOverflow) {
  std::istringstream in(R"({"operation":"fact","args":[50]})");
  std::ostringstream out, err;

  Runner r;
  int code = r.run(in, out, err);

  EXPECT_EQ(code, 1);
}

TEST(InOutTest, UnknownOperation) {
  std::istringstream in(R"({"operation":"xxx","args":[1,2]})");
  std::ostringstream out, err;

  Runner r;
  int code = r.run(in, out, err);

  EXPECT_EQ(code, 1);
}
