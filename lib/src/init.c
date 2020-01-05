#include "init.h"

#include "event_time.h"
#include "global_buffer.h"
#include "worker_thread.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

static FILE* output_file;

void syan_init() {
  int event_time_init_status = syan_event_time_init();
  if (event_time_init_status != 0) {
    fprintf(stderr,
            "SyncAnalysis init: system does not support monotonic clock "
            "(clock_gettime errno=%d)\n",
            event_time_init_status);
    exit(EXIT_FAILURE);
  }

  BufferInitStatus buffer_init_status = syan_global_buffer_init();
  if (buffer_init_status == BUFFER_INIT_MALLOC_FAILED_BUFFER) {
    fprintf(stderr, "SyncAnalysis init: malloc failed for %lu bytes\n",
            sizeof(Buffer));
    exit(EXIT_FAILURE);
  }
  if (buffer_init_status == BUFFER_INIT_MALLOC_FAILED_PAGE) {
    fprintf(stderr, "SyncAnalysis init: malloc failed for %lu bytes\n",
            sizeof(BufferPage));
    exit(EXIT_FAILURE);
  }

  output_file = fopen("sync_analysis.dump", "wb");
  if (output_file == NULL) {
    fprintf(stderr, "SyncAnalysis init: failed to open output file. errno=%d\n",
            errno);
    exit(EXIT_FAILURE);
  }

  int status = syan_start_worker_thread(output_file);
  if (status != 0) {
    fprintf(stderr, "SyncAnalysis init: pthread_create failed error=%d\n",
            status);
    exit(EXIT_FAILURE);
  }
}

void syan_shutdown() {
  int status = syan_stop_worker_thread();
  if (status != 0) {
    fprintf(stderr, "SyncAnalysis shutdown: pthread_join failed error=%d\n",
            status);
    exit(EXIT_FAILURE);
  }

  fclose(output_file);
}
