#include "worker_thread.h"

#include "global_buffer.h"

#include <errno.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <unistd.h>

static pthread_t worker_thread;
static atomic_bool stop_token;

static void write_all_or_crash(void* buffer, int num_bytes, int fd) {
  while (num_bytes > 0) {
    int num_written = write(fd, buffer, num_bytes);
    if (num_written < 0) {
      fprintf(stderr, "SyncAnalysis write: failed with error %d\n", errno);
      exit(EXIT_FAILURE);
    }
    num_bytes -= num_written;
    buffer += num_written;
  }
}

static bool work(int output_fd) {
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
                     output_fd);

  if (back == SYAN_BUFFER_PAGE_SIZE) {
    syan_global_buffer_release_front_page();
  } else {
    buf_page->storage_front = back;
  }

  return back - front > 0;
}

static void* loop(void* file_descriptor) {
  int fd = *(int*)file_descriptor;
  free(file_descriptor);
  while (atomic_load_explicit(&stop_token, memory_order_acquire) == false) {
    while (work(fd)) {
    }
    usleep(5);
  }
  return NULL;
}

int syan_start_worker_thread(void* file_descriptor) {
  atomic_store_explicit(&stop_token, false, memory_order_release);
  pthread_attr_t thread_attr;
  return pthread_create(&worker_thread, &thread_attr, loop, file_descriptor);
}

int syan_stop_worker_thread() {
  atomic_store_explicit(&stop_token, true, memory_order_release);
  void* thread_output;
  return pthread_join(worker_thread, &thread_output);
}
