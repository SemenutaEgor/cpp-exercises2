#pragma once

#include <stdexcept>
#include <string>

class AppError : public std::runtime_error {
 public:
  explicit AppError(const std::string& message) : std::runtime_error(message) {}
};

class ParseError : public AppError {
 public:
  explicit ParseError(const std::string& message) : AppError(message) {}
};

class ValidationError : public AppError {
 public:
  explicit ValidationError(const std::string& message) : AppError(message) {}
};

class CalculationError : public AppError {
 public:
  explicit CalculationError(const std::string& message) : AppError(message) {}
};
