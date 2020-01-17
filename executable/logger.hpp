#ifndef SYNC_ANALYSIS_EXE_LOGGER_H_
#define SYNC_ANALYSIS_EXE_LOGGER_H_

#include <string>

namespace syan {

class Logger {
public:
  [[noreturn]] void log_fatal(const std::string& message);
};

}

#endif
