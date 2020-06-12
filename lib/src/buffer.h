#ifndef SYNC_ANALYSIS_LIB_SRC_BUFFER_H_
#define SYNC_ANALYSIS_LIB_SRC_BUFFER_H_

#include <stdatomic.h>

#include "event.h"

#define SYAN_BUFFER_PAGE_SIZE ((int)((1u << 15u) / sizeof(SyanEvent)))

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  // 32KB in each buffer page.
  SyanEvent storage[SYAN_BUFFER_PAGE_SIZE];
  int_fast32_t storage_front;
  atomic_int_fast32_t storage_back;
} SyanBufferPage;

typedef SyanBufferPage* SyanBufferPagePtr;

#define SYAN_BUFFER_NUM_PAGES ((int)(1u << 10u))

typedef struct {
  // Cannot have more than 1024 pages in memory.
  SyanBufferPagePtr pages[SYAN_BUFFER_NUM_PAGES];
  int_fast16_t pages_front;
  atomic_int_fast16_t pages_back;
} SyanBuffer;

enum {
  SYAN_BUFFER_INIT_OK,
  SYAN_BUFFER_INIT_MALLOC_BUFFER_FAILED,
  SYAN_BUFFER_INIT_MALLOC_PAGE_FAILED,
};

int syan_buffer_init(SyanBuffer** buffer);

SyanEvent* syan_buffer_acquire_event_slot(SyanBuffer* buffer);

SyanBufferPagePtr syan_buffer_get_front_page(SyanBuffer* buffer);

void syan_buffer_release_front_page(SyanBuffer* buffer);

#ifdef __cplusplus
}
#endif

#endif
