#ifndef SYNC_ANALYSIS_EXECUTABLE_SRC_DEBUG_H_
#define SYNC_ANALYSIS_EXECUTABLE_SRC_DEBUG_H_

#include <iostream>
#include <string>

namespace syan::debugging {

struct NewlineAppender {
  std::ostream& out;

  ~NewlineAppender();

  template<class T>
  NewlineAppender& operator<<(T&& value) {
    out << value;
    return *this;
  }

  explicit operator bool() const;
};

struct FatalAbortStream {
  std::ostream& out;

  [[noreturn]] ~FatalAbortStream();

  template<class T>
  FatalAbortStream& operator<<(T&& value) {
    out << value;
    return *this;
  }
};

std::string formatted_time();

void install_abort_handler();

void set_debug_enabled(bool enabled);

bool is_debug_enabled();

}  // namespace syan::debugging

#define SYAN_CURRENT_FILE                                                      \
  (__builtin_strrchr(__FILE__, '/')                                            \
       ? __builtin_strrchr(__FILE__, '/') + 1                                  \
       : __builtin_strrchr(__FILE__, '\\')                                     \
             ? __builtin_strrchr(__FILE__, '\\') + 1                           \
             : __FILE__)

#define DOUT                                                                   \
  ::syan::debugging::is_debug_enabled() &&                                     \
      ::syan::debugging::NewlineAppender{::std::cout}                          \
          << "[DBG " << ::syan::debugging::formatted_time() << " "             \
          << SYAN_CURRENT_FILE << ":" << __LINE__ << "] "

#define FATAL_OUT                                                              \
  ::syan::debugging::FatalAbortStream{::std::cout}                             \
      << " FATAL (" << SYAN_CURRENT_FILE << ":" << __LINE__ << ") "

#endif
