#include "buffer.h"

static SyanBuffer* syan_global_buffer = NULL;

SyanBufferInitStatus syan_global_buffer_init() {
  return syan_buffer_init(&syan_global_buffer);
}

SyanEvent* syan_global_buffer_acquire_event_slot() {
  return syan_buffer_acquire_event_slot(syan_global_buffer);
}

SyanBufferPagePtr syan_global_buffer_get_front_page() {
  return syan_buffer_get_front_page(syan_global_buffer);
}

void syan_global_buffer_release_front_page() {
  syan_buffer_release_front_page(syan_global_buffer);
}
