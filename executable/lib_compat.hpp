#ifndef SYNC_ANALYSIS_EXE_LIB_COMPAT_H_
#define SYNC_ANALYSIS_EXE_LIB_COMPAT_H_

#include <string>

#include <lib/src/dump_file_header.h>
#include <lib/src/event.h>
#include <lib/src/event_type.h>

namespace syan {

struct DumpFileHeader {
  struct timespec start_time;
  std::string program_name;
  std::string program_command;

// Platform-specific stuff.
#ifdef SYNC_ANALYSIS_IS_MAC_OS_X
  std::intptr_t program_load_addr;
#endif

  DumpFileHeader() = default;
  DumpFileHeader(DumpFileHeader&&) noexcept = default;
  DumpFileHeader(const DumpFileHeader&) = default;

  void load(::SyanDumpFileHeader&& raw_header) {
    start_time = raw_header.start_time;

    program_name = raw_header.program_name;
    free((void*)raw_header.program_name);
    raw_header.program_name = nullptr;

    program_command = raw_header.program_command;
    free((void*)raw_header.program_command);
    raw_header.program_command = nullptr;

#ifdef SYNC_ANALYSIS_IS_MAC_OS_X
    program_load_addr = raw_header.program_load_addr;
#endif
  }
};

}  // namespace syan

#endif
