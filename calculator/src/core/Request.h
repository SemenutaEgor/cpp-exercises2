#pragma once

#include <string>
#include <vector>

class Request {
 public:
  Request(std::string operation, std::vector<int> args)
      : operation_(std::move(operation)), args_(std::move(args)) {}

  const std::string& operation() const noexcept { return operation_; }

  const std::vector<int>& args() const noexcept { return args_; }

 private:
  std::string operation_;
  std::vector<int> args_;
};
