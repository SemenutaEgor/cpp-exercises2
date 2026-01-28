#pragma once
#include <string>

enum class OpType { ADD, SUB, MUL, DIV, POW, FACT, UNKNOWN };

inline OpType opFromString(const std::string& op) {
  if (op == "add") return OpType::ADD;
  if (op == "sub") return OpType::SUB;
  if (op == "mul") return OpType::MUL;
  if (op == "div") return OpType::DIV;
  if (op == "pow") return OpType::POW;
  if (op == "fact") return OpType::FACT;
  return OpType::UNKNOWN;
}
