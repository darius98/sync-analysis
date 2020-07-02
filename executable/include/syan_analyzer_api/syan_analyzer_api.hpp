#ifndef SYNC_ANALYSIS_EXECUTABLE_INCLUDE_SYAN_ANALYZER_API_SYAN_ANALYZER_API_H_
#define SYNC_ANALYSIS_EXECUTABLE_INCLUDE_SYAN_ANALYZER_API_SYAN_ANALYZER_API_H_

#include <syan_analyzer_api/database.hpp>
#include <syan_analyzer_api/event.hpp>
#include <syan_analyzer_api/report.hpp>

namespace syan {

Event current_event();

const Database& database();

Report create_report();

}  // namespace syan

#if __GNUC__ >= 4
#define SYAN_ANALYZER_API extern "C" [[maybe_unused]]
#else
#define SYAN_ANALYZER_API extern "C" [[maybe_unused]]
#endif

#define SYAN_ANALYZER_CLASS(cls)                                               \
  namespace {                                                                  \
  cls* instance = nullptr;                                                     \
  }                                                                            \
  SYAN_ANALYZER_API void syan_analyzer_start_up() {                            \
    instance = new cls();                                                      \
  }                                                                            \
  SYAN_ANALYZER_API void syan_analyzer_on_event() {                            \
    (*instance)(::syan::current_event());                                      \
  }                                                                            \
  SYAN_ANALYZER_API void syan_analyzer_shut_down() {                           \
    delete instance;                                                           \
    instance = nullptr;                                                        \
  }                                                                            \
  SYAN_ANALYZER_API const char* syan_analyzer = cls::name

#endif
