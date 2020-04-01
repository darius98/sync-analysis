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

std::string formatted_time();

void install_abort_handler();

}  // namespace syan::debugging

#define debug_cout                                                             \
  ::syan::debugging::NewlineAppender{::std::cout}                              \
      << "(debug, " << ::syan::debugging::formatted_time() << ") "
#else
#define debug_cout false && ::std::cout
#endif

#endif
