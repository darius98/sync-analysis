#include "worker_thread.h"

#include "global_buffer.h"

#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <unistd.h>

static pthread_t worker_thread;
static atomic_bool stop_token;

static void write_all_or_crash(void* buffer, int num_bytes, FILE* file) {
  int num_written = fwrite(buffer, 1, num_bytes, file);
  if (num_written < num_bytes) {
    fprintf(stderr, "SyncAnalysis write: failed with error %d\n", ferror(file));
    exit(EXIT_FAILURE);
  }
}

static bool work(FILE* file) {
  BufferPagePtr buf_page = syan_global_buffer_get_front_page();
  int_fast32_t front = buf_page->storage_front;
  int_fast32_t back =
      atomic_load_explicit(&buf_page->storage_back, memory_order_acquire);
  if (back > SYAN_BUFFER_PAGE_SIZE) {
    back = SYAN_BUFFER_PAGE_SIZE;
  }

  for (int i = front; i < back; i++) {
    while (atomic_load_explicit(&buf_page->storage[i].signature,
                                memory_order_acquire) != SYAN_EVENT_SIGNATURE) {
    }
  }

  write_all_or_crash(&buf_page->storage[front], (back - front) * sizeof(Event),
                     file);

  if (back == SYAN_BUFFER_PAGE_SIZE) {
    syan_global_buffer_release_front_page();
  } else {
    buf_page->storage_front = back;
  }

  return back - front > 0;
}

static void* loop(void* param) {
  FILE* file = (FILE*)param;
  while (atomic_load_explicit(&stop_token, memory_order_acquire) == false) {
    while (work(file)) {
    }
    usleep(5);
  }
  return NULL;
}

int syan_start_worker_thread(void* param) {
  atomic_store_explicit(&stop_token, false, memory_order_release);
  return pthread_create(&worker_thread, NULL, loop, param);
}

int syan_stop_worker_thread() {
  atomic_store_explicit(&stop_token, true, memory_order_release);
  void* thread_output;
  return pthread_join(worker_thread, &thread_output);
}
