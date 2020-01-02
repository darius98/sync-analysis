#ifndef SYNC_ANALYSIS_LIB_SRC_GLOBAL_BUFFER_H_
#define SYNC_ANALYSIS_LIB_SRC_GLOBAL_BUFFER_H_

#include "buffer.h"

BufferInitStatus syan_global_buffer_init();

Event* syan_global_buffer_acquire_event_slot();

BufferPagePtr syan_global_buffer_get_front_page();

void syan_global_buffer_release_front_page();

#endif
