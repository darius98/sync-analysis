#include "logger.hpp"

#include <cstdlib>
#include <iostream>

namespace syan {

void Logger::log_info(const std::string& message) const {
  std::cout << "INFO: " << message << "\n";
}

void Logger::log_warn(const std::string& message) const {
  std::cout << "WARNING: " << message << "\n";
}

void Logger::log_error(const std::string& message) const {
  std::cout << "ERROR: " << message << "\n";
}

void Logger::log_fatal(const std::string& message) const {
  std::cout << "FATAL ERROR: " << message << "\n";
  std::abort();
}

} // namespace syan
