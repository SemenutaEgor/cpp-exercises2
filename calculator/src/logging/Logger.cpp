#include "logging/Logger.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

Logger& Logger::instance() {
  static Logger instance;
  return instance;
}

Logger::Logger() {
  auto logger = spdlog::stdout_color_mt("calculator");
  logger->set_level(spdlog::level::debug);
  spdlog::set_default_logger(logger);
}

void Logger::info(const std::string& msg) {
  spdlog::info(msg);
}

void Logger::error(const std::string& msg) {
  spdlog::error(msg);
}

void Logger::debug(const std::string& msg) {
  spdlog::debug(msg);
}
