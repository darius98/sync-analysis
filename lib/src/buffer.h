#ifndef SYNC_ANALYSIS_LIB_SRC_BUFFER_H_
#define SYNC_ANALYSIS_LIB_SRC_BUFFER_H_

#include <stdatomic.h>

#include "event.h"

typedef struct {
  // 32KB in each buffer page.
  Event storage[(1u << 15u) / sizeof(Event)];
  int_fast32_t storage_front;
  atomic_int_fast32_t storage_back;
} BufferPage;

typedef BufferPage* BufferPagePtr;

typedef struct {
  // Cannot have more than 1024 pages in memory.
  BufferPagePtr pages[1u << 10u];
  atomic_int_fast16_t pages_allocated;

  BufferPagePtr pages_queue[1u << 10u];
  atomic_int_fast16_t pages_queue_front;
  atomic_int_fast16_t pages_queue_back;
} Buffer;

typedef enum {
  BUFFER_INIT_OK,
  BUFFER_INIT_MALLOC_FAILED_BUFFER,
  BUFFER_INIT_MALLOC_FAILED_FIRST_PAGE,
} BufferInitStatus;

BufferInitStatus syan_buffer_init(Buffer** buffer);

Event* syan_buffer_acquire_event_slot(Buffer* buffer);

BufferPagePtr syan_buffer_get_front_page(Buffer* buffer);

void syan_buffer_release_front_page(Buffer* buffer);

#endif
