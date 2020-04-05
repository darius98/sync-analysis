#ifndef SYNC_ANALYSIS_EXECUTABLE_INCLUDE_SYAN_EXTENSION_API_SYAN_EXTENSION_API_H_
#define SYNC_ANALYSIS_EXECUTABLE_INCLUDE_SYAN_EXTENSION_API_SYAN_EXTENSION_API_H_

#include <syan_extension_api/database.hpp>
#include <syan_extension_api/event.hpp>
#include <syan_extension_api/report.hpp>

namespace syan {

Event current_event();

const Database& database();

Report create_report();

}  // namespace syan

#if __GNUC__ >= 4
#define SYAN_EXT_API                                                           \
  __attribute__((visibility("default"))) extern "C" [[maybe_unused]]
#else
#define SYAN_EXT_API extern "C" [[maybe_unused]]
#endif

#define SYAN_EXTENSION_SET_CLASS(cls)                                          \
  namespace {                                                                  \
  cls* instance = nullptr;                                                     \
  }                                                                            \
  SYAN_EXT_API void syan_extension_start_up() {                                \
    instance = new cls();                                                      \
  }                                                                            \
  SYAN_EXT_API void syan_extension_on_event() {                                \
    (*instance)(::syan::current_event());                                      \
  }                                                                            \
  SYAN_EXT_API void syan_extension_shut_down() {                               \
    delete instance;                                                           \
    instance = nullptr;                                                        \
  }                                                                            \
  SYAN_EXT_API const char* syan_extension = cls::name

#endif
