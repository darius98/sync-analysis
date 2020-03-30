#ifndef SYNC_ANALYSIS_EXECUTABLE_SRC_DEBUG_H_
#define SYNC_ANALYSIS_EXECUTABLE_SRC_DEBUG_H_

#include <iostream>

#ifdef SYNC_ANALYSIS_DEBUG_MODE

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace syan::debugging {

struct NewlineAppender {
  std::ostream& out;

  ~NewlineAppender() {
    out << std::endl;
  }

  template<class T>
  NewlineAppender& operator<<(T&& value) {
    out << value;
    return *this;
  }
};

inline std::string formatted_time() {
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

}  // namespace syan::debugging

#define debug_cout                                                             \
  ::syan::debugging::NewlineAppender{::std::cout}                              \
      << "(debug, " << ::syan::debugging::formatted_time() << ") "
#else
#define debug_cout false && ::std::cout
#endif

#endif
