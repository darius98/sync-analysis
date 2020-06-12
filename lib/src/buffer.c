#include "buffer.h"

#include <stdlib.h>
#include <string.h>

static void syan_reset_buffer_page(SyanBufferPagePtr page) {
  page->storage_front = 0;
  atomic_store(&page->storage_back, 0);
  memset(page->storage, 0, sizeof(page->storage));
}

SyanBufferInitStatus syan_buffer_init(SyanBuffer** buffer) {
  *buffer = malloc(sizeof(SyanBuffer));
  if (*buffer == NULL) {
    return BUFFER_INIT_MALLOC_FAILED_BUFFER;
  }

  for (int i = 0; i < SYAN_BUFFER_NUM_PAGES; i++) {
    (*buffer)->pages[i] = malloc(sizeof(SyanBufferPage));
    if ((*buffer)->pages[i] == NULL) {
      for (int j = 0; j < i; j++) {
        free((*buffer)->pages[j]);
      }
      free(*buffer);
      return BUFFER_INIT_MALLOC_FAILED_PAGE;
    }
    syan_reset_buffer_page((*buffer)->pages[i]);
  }
  (*buffer)->pages_front = 0;
  atomic_store(&(*buffer)->pages_back, 0);

  return BUFFER_INIT_OK;
}

SyanEvent* syan_buffer_acquire_event_slot(SyanBuffer* buffer) {
  while (1) {
    int_fast16_t page_index = atomic_load(&buffer->pages_back);
    SyanBufferPagePtr page = buffer->pages[page_index];
    int_fast32_t slot = atomic_fetch_add(&page->storage_back, 1);
    if (slot < SYAN_BUFFER_PAGE_SIZE) {
      return page->storage + slot;
    }
    int_fast16_t next_page_index = page_index + 1;
    if (next_page_index == SYAN_BUFFER_NUM_PAGES) {
      next_page_index = 0;
    }
    atomic_compare_exchange_strong(&buffer->pages_back, &page_index,
                                   next_page_index);
  }
}

SyanBufferPagePtr syan_buffer_get_front_page(SyanBuffer* buffer) {
  return buffer->pages[buffer->pages_front];
}

void syan_buffer_release_front_page(SyanBuffer* buffer) {
  SyanBufferPagePtr page = syan_buffer_get_front_page(buffer);
  buffer->pages_front++;
  if (buffer->pages_front == SYAN_BUFFER_NUM_PAGES) {
    buffer->pages_front = 0;
  }
  syan_reset_buffer_page(page);
}
