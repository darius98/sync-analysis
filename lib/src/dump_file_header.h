#ifndef SYNC_ANALYSIS_LIB_SRC_DUMP_FILE_HEADER_H_
#define SYNC_ANALYSIS_LIB_SRC_DUMP_FILE_HEADER_H_

#include <stdint.h>
#include <stdio.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  struct timespec start_time;
  size_t program_name_length;
  const char* program_name;
  size_t program_command_length;
  const char* program_command;

// Platform-specific stuff.
#ifdef SYNC_ANALYSIS_IS_MAC_OS_X
  intptr_t program_load_addr;
#endif
} SyanDumpFileHeader;

int syan_init_dump_file_header(SyanDumpFileHeader* header, int argc,
                               char** argv);

int syan_write_dump_file_header(FILE* stream, const SyanDumpFileHeader* header);

int syan_read_dump_file_header(FILE* stream, SyanDumpFileHeader* header);

#ifdef __cplusplus
}
#endif

#endif
