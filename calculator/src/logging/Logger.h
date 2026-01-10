#pragma once
#include <string>

class Logger {
public:
  static Logger& instance();

  void info(const std::string& msg);
  void error(const std::string& msg);
  void debug(const std::string& msg);

private:
  Logger();
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;
};
