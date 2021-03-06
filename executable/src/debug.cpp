#include "debug.hpp"

#include <cstring>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include <execinfo.h>

#include <csignal>

namespace {

bool debug_flag = false;

}

namespace syan::debugging {

NewlineAppender::~NewlineAppender() {
  out << std::endl;
}

NewlineAppender::operator bool() const {
  return static_cast<bool>(out);
}

FatalAbortStream::~FatalAbortStream() {
  out << std::endl;
  std::abort();
}

std::string formatted_time() {
  auto timestamp = std::chrono::system_clock::now();
  auto timestamp_c_style = std::chrono::system_clock::to_time_t(timestamp);
  auto timestamp_nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(
                             timestamp.time_since_epoch())
                             .count();
  tm* calendar_time = gmtime(&timestamp_c_style);
  std::stringstream formatter;
  formatter << std::put_time(calendar_time, "%H:%M:%S") << "."
            << std::setfill('0') << std::setw(6)
            << (timestamp_nanos % 1000000000) / 1000;
  return formatter.str();
}

void install_abort_handler() {
  std::signal(SIGABRT, [](int signal) {
    if (signal == SIGABRT) {
      std::cerr << "SIGABRT received\n";
      void* trace[32];
      backtrace(trace, 32);
      char** trace_symbols = backtrace_symbols(trace, 32);
      for (int i = 0; i < 32; i++) {
        if (trace[i] != nullptr) {
          std::cerr << "\t" << trace_symbols[i] << "\n";
          if (strstr(trace_symbols[i], " start ") != nullptr) {
            break;
          }
        }
      }
    } else {
      std::cerr << "Unexpected signal " << signal << " received\n";
    }
  });
}

void set_debug_enabled(bool enabled) {
  debug_flag = enabled;
  if (enabled) {
    syan::debugging::install_abort_handler();
  }
}

bool is_debug_enabled() {
  return debug_flag;
}

}  // namespace syan::debugging
