#include "global_buffer.h"

#include <errno.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <unistd.h>

static pthread_t syan_worker_thread;
static atomic_bool syan_stop_token;

static bool work(FILE* file) {
  SyanBufferPagePtr buf_page = syan_global_buffer_get_front_page();
  int_fast32_t front = buf_page->storage_front;
  int_fast32_t back = atomic_load(&buf_page->storage_back);
  if (back > SYAN_BUFFER_PAGE_SIZE) {
    back = SYAN_BUFFER_PAGE_SIZE;
  }

  for (int i = front; i < back; i++) {
    int32_t ret;
    do {
      __atomic_load(&buf_page->storage[i].signature, &ret, __ATOMIC_ACQUIRE);
    } while (ret != SYAN_EVENT_SIGNATURE);
  }

  int num_bytes = (back - front) * sizeof(SyanEvent);
  int num_written = fwrite(&buf_page->storage[front], 1, num_bytes, file);
  if (num_written < num_bytes) {
    fprintf(stderr,
            "SyncAnalysis write failed: num_events=%d, num_bytes=%d, "
            "ferror=%d, errno=%d\n",
            (int)(back - front), num_bytes, ferror(file), errno);
    exit(EXIT_FAILURE);
  }

  if (back == SYAN_BUFFER_PAGE_SIZE) {
    syan_global_buffer_release_front_page();
  } else {
    buf_page->storage_front = back;
  }

  return back - front > 0;
}

static void* loop(void* param) {
  FILE* file = (FILE*)param;
  while (!atomic_load(&syan_stop_token)) {
    while (work(file)) {
    }
    usleep(5);
  }
  while (work(file)) {
  }
  return NULL;
}

int syan_start_worker_thread(void* param) {
  atomic_store(&syan_stop_token, false);
  return pthread_create(&syan_worker_thread, NULL, loop, param);
}

int syan_stop_worker_thread() {
  atomic_store(&syan_stop_token, true);
  void* thread_output;
  return pthread_join(syan_worker_thread, &thread_output);
}
