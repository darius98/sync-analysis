#ifndef SYNC_ANALYSIS_LIB_SRC_BUFFER_H_
#define SYNC_ANALYSIS_LIB_SRC_BUFFER_H_

#include <stdatomic.h>

#include "event.h"

#define SYAN_BUFFER_PAGE_SIZE ((1u << 15u) / sizeof(Event))

typedef struct {
  // 32KB in each buffer page.
  Event storage[SYAN_BUFFER_PAGE_SIZE];
  int_fast32_t storage_front;
  atomic_int_fast32_t storage_back;
} BufferPage;

typedef BufferPage* BufferPagePtr;

#define SYAN_BUFFER_NUM_PAGES (1u << 10u)

typedef struct {
  // Cannot have more than 1024 pages in memory.
  BufferPagePtr pages[SYAN_BUFFER_NUM_PAGES];
  int_fast16_t pages_front;
  atomic_int_fast16_t pages_back;
} Buffer;

typedef enum {
  BUFFER_INIT_OK,
  BUFFER_INIT_MALLOC_FAILED_BUFFER,
  BUFFER_INIT_MALLOC_FAILED_PAGE,
} BufferInitStatus;

BufferInitStatus syan_buffer_init(Buffer** buffer);

Event* syan_buffer_acquire_event_slot(Buffer* buffer);

BufferPagePtr syan_buffer_get_front_page(Buffer* buffer);

void syan_buffer_release_front_page(Buffer* buffer);

#endif
