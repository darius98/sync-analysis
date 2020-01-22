#ifndef SYNC_ANALYSIS_LIB_SRC_DUMP_FILE_HEADER_H_
#define SYNC_ANALYSIS_LIB_SRC_DUMP_FILE_HEADER_H_

#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  struct timespec start_time;
} SyanDumpFileHeader;

int syan_init_dump_file_header(SyanDumpFileHeader* header);

#ifdef __cplusplus
}
#endif

#endif
