#include "dump_file_header.h"
#include "event_time.h"
#include "global_buffer.h"
#include "sync_analysis.h"
#include "worker_thread.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

static FILE* syan_output_file;

void syan_init(int argc, char** argv) {
  int event_time_init_status = syan_event_time_init();
  if (event_time_init_status != 0) {
    fprintf(stderr,
            "SyncAnalysis init: system does not support monotonic clock "
            "(clock_gettime errno=%d)\n",
            event_time_init_status);
    exit(EXIT_FAILURE);
  }

  SyanBufferInitStatus buffer_init_status = syan_global_buffer_init();
  if (buffer_init_status == BUFFER_INIT_MALLOC_FAILED_BUFFER) {
    fprintf(stderr, "SyncAnalysis init: malloc failed for %lu bytes\n",
            sizeof(SyanBuffer));
    exit(EXIT_FAILURE);
  }
  if (buffer_init_status == BUFFER_INIT_MALLOC_FAILED_PAGE) {
    fprintf(stderr, "SyncAnalysis init: malloc failed for %lu bytes\n",
            sizeof(SyanBufferPage));
    exit(EXIT_FAILURE);
  }

  syan_output_file = fopen("sync_analysis.dump", "wb");
  if (syan_output_file == NULL) {
    fprintf(stderr, "SyncAnalysis init: failed to open output file. errno=%d\n",
            errno);
    exit(EXIT_FAILURE);
  }

  SyanDumpFileHeader header;
  int init_header_status = syan_init_dump_file_header(&header, argc, argv);
  if (init_header_status != 0) {
    fprintf(
        stderr,
        "SyncAnalysis init: failed to initialize dump file header. error=%d\n",
        init_header_status);
    exit(EXIT_FAILURE);
  }

  int write_header_status =
      syan_write_dump_file_header(syan_output_file, &header);
  if (write_header_status != 0) {
    fprintf(stderr,
            "SyncAnalysis init: failed to write dump file header. ferror=%d, "
            "errno=%d\n",
            ferror(syan_output_file), errno);
    exit(EXIT_FAILURE);
  }

  int status = syan_start_worker_thread(syan_output_file);
  if (status != 0) {
    fprintf(stderr, "SyncAnalysis init: pthread_create failed error=%d\n",
            status);
    exit(EXIT_FAILURE);
  }

  syan_capture_event(SYNC_ANALYSIS_THREAD_CREATE_EVENT_TYPE, pthread_self());
}

void syan_shutdown() {
  int status = syan_stop_worker_thread();
  if (status != 0) {
    fprintf(stderr, "SyncAnalysis shutdown: pthread_join failed error=%d\n",
            status);
    exit(EXIT_FAILURE);
  }

  fclose(syan_output_file);
}
