#ifndef SYNC_ANALYSIS_EXECUTABLE_INCLUDE_SYAN_EXTENSION_API_SYAN_EXTENSION_API_H_
#define SYNC_ANALYSIS_EXECUTABLE_INCLUDE_SYAN_EXTENSION_API_SYAN_EXTENSION_API_H_

#include <syan_extension_api/database.hpp>
#include <syan_extension_api/event.hpp>
#include <syan_extension_api/report.hpp>

#if defined _WIN32 || defined __CYGWIN__
#ifdef __GNUC__
#define SYAN_EXT_API __attribute__((dllexport)) extern "C"
#else
#define SYAN_EXT_API __declspec(dllexport) extern "C"
#endif
#else
#if __GNUC__ >= 4
#define SYAN_EXT_API __attribute__((visibility("default"))) extern "C"
#else
#define SYAN_EXT_API extern "C"
#endif
#endif

/*

[[required]] const char* syan_extension;

void syan_extension_init();

void syan_extension_on_event(syan::Event);

void syan_extension_destroy();

*/

#endif
