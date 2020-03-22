#ifndef SYNC_ANALYSIS_EXE_LIB_COMPAT_H_
#define SYNC_ANALYSIS_EXE_LIB_COMPAT_H_

#include <string>

#include <lib/src/dump_file_header.h>

namespace syan {

struct DumpFileHeader {
  struct timespec start_time;

// Platform-specific stuff.
#ifdef SYNC_ANALYSIS_IS_MAC_OS_X
  std::intptr_t program_load_addr;
#endif

  std::string program_name;
  std::string program_command;

  DumpFileHeader() = default;
  DumpFileHeader(DumpFileHeader&&) noexcept = default;
  DumpFileHeader(const DumpFileHeader&) = default;

  explicit DumpFileHeader(::SyanDumpFileHeader&& raw_header)
      : start_time(raw_header.start_time),
#ifdef SYNC_ANALYSIS_IS_MAC_OS_X
        program_load_addr(raw_header.program_load_addr),
#endif
        program_name(raw_header.program_name),
        program_command(raw_header.program_command) {
    free((void*)raw_header.program_name);
    raw_header.program_name = nullptr;

    free((void*)raw_header.program_command);
    raw_header.program_command = nullptr;
  }
};

}  // namespace syan

#endif
