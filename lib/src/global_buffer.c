#include "global_buffer.h"

#include "buffer.h"

static Buffer* buffer = NULL;

BufferInitStatus syan_global_buffer_init() {
  return syan_buffer_init(&buffer);
}

Event* syan_global_buffer_acquire_event_slot() {
  return syan_buffer_acquire_event_slot(buffer);
}

BufferPagePtr syan_global_buffer_get_front_page() {
  return syan_buffer_get_front_page(buffer);
}

void syan_global_buffer_release_front_page() {
  syan_buffer_release_front_page(buffer);
}
