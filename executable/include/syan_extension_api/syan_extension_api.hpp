#ifndef SYNC_ANALYSIS_EXECUTABLE_INCLUDE_SYAN_EXTENSION_API_SYAN_EXTENSION_API_H_
#define SYNC_ANALYSIS_EXECUTABLE_INCLUDE_SYAN_EXTENSION_API_SYAN_EXTENSION_API_H_

#include <syan_extension_api/database.hpp>
#include <syan_extension_api/event.hpp>
#include <syan_extension_api/report.hpp>

#if defined _WIN32 || defined __CYGWIN__
#ifdef __GNUC__
#define SYAN_EXT_API __attribute__((dllexport)) extern "C" [[maybe_unused]]
#else
#define SYAN_EXT_API __declspec(dllexport) extern "C" [[maybe_unused]]
#endif
#else
#if __GNUC__ >= 4
#define SYAN_EXT_API                                                           \
  __attribute__((visibility("default"))) extern "C" [[maybe_unused]]
#else
#define SYAN_EXT_API extern "C" [[maybe_unused]]
#endif
#endif

namespace syan {

Event current_event() noexcept;

const Database& database() noexcept;

Report create_report();

}  // namespace syan

/*
Extension ABI:

const char* syan_extension; (REQUIRED)
void syan_extension_init();
void syan_extension_on_event(syan::Event);
void syan_extension_destroy();

Everything symbol must have C linkage to be detected.

*/

#endif
