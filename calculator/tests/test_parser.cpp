#include <gtest/gtest.h>

#include <sstream>

#include "core/Errors.h"
#include "core/Parser.h"

TEST(ParserTest, ValidInput) {
  std::istringstream input(R"({"operation":"add","args":[1,2]})");

  Parser parser;
  Request req = parser.parse(input);

  EXPECT_EQ(req.operation(), "add");
  ASSERT_EQ(req.args().size(), 2);
  EXPECT_EQ(req.args()[0], 1);
  EXPECT_EQ(req.args()[1], 2);
}

TEST(ParserTest, MissingOperation) {
  std::istringstream input(R"({"args":[1,2]})");

  Parser parser;
  EXPECT_THROW(parser.parse(input), ParseError);
}

TEST(ParserTest, MissingArgs) {
  std::istringstream input(R"({"operation":"add"})");

  Parser parser;
  EXPECT_THROW(parser.parse(input), ParseError);
}

TEST(ParserTest, NonIntegerArgs) {
  std::istringstream input(R"({"operation":"fact","args":[1.0, 2.44]})");

  Parser parser;
  EXPECT_THROW(parser.parse(input), ParseError);
}

TEST(ParserTest, InvalidJson) {
  std::istringstream input(R"({"operation":"add","args":[1,2])");

  Parser parser;
  EXPECT_THROW(parser.parse(input), ParseError);
}

TEST(ParserTest, EmptyInput) {
  std::istringstream input("");

  Parser parser;
  EXPECT_THROW(parser.parse(input), ParseError);
}

TEST(ParserTest, OperationIsNotString) {
  std::istringstream input(R"({"operation":123,"args":[1,2]})");

  Parser parser;
  EXPECT_THROW(parser.parse(input), ParseError);
}

TEST(ParserTest, ArgsIsNotArray) {
  std::istringstream input(R"({"operation":"add","args":42})");

  Parser parser;
  EXPECT_THROW(parser.parse(input), ParseError);
}

TEST(ParserTest, EmptyArgsArrayIsAllowed) {
  std::istringstream input(R"({"operation":"add","args":[]})");

  Parser parser;
  EXPECT_NO_THROW(parser.parse(input));
}

TEST(ParserTest, ExtraFieldsAreIgnored) {
  std::istringstream input(R"({
    "operation":"add",
    "args":[1,2],
    "extra":"something"
  })");

  Parser parser;
  EXPECT_NO_THROW(parser.parse(input));
}

TEST(ParserTest, LargeInteger) {
  std::istringstream input(R"({"operation":"fact","args":[2147483647]})");

  Parser parser;
  EXPECT_NO_THROW(parser.parse(input));
}
