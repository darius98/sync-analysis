#include "logger.hpp"

#include <cstdlib>
#include <iostream>

namespace syan {

void Logger::log_fatal(const std::string& message) {
  std::cout << message << "\n";
  std::abort();
}

} // namespace syan
