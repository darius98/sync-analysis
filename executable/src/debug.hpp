#ifndef SYNC_ANALYSIS_EXECUTABLE_SRC_DEBUG_H_
#define SYNC_ANALYSIS_EXECUTABLE_SRC_DEBUG_H_

#include <iostream>

#ifdef SYNC_ANALYSIS_DEBUG_MODE

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

#define debug_cout                                                             \
  ::syan::debugging::is_debug_enabled() &&                                     \
      ::syan::debugging::NewlineAppender{::std::cout}                          \
          << "[" << ::syan::debugging::formatted_time() << " "                 \
          << SYAN_CURRENT_FILE << ":" << __LINE__ << "] "
#else
#define debug_cout false && ::std::cout
#endif

#endif
