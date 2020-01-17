#ifndef SYNC_ANALYSIS_EXE_LOGGER_H_
#define SYNC_ANALYSIS_EXE_LOGGER_H_

#include <string>

namespace syan {

class Logger {
public:
  void log_info(const std::string& message) const;

  void log_warn(const std::string& message) const;

  void log_error(const std::string& message) const;

  [[noreturn]] void log_fatal(const std::string& message) const;
};

}

#endif
