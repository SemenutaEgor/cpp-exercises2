#pragma once

class Request;
class Result;

class Calculator {
 public:
  Result calculate(const Request&);
};