#ifndef SYNC_ANALYSIS_LIB_SRC_GLOBAL_BUFFER_H_
#define SYNC_ANALYSIS_LIB_SRC_GLOBAL_BUFFER_H_

#include "buffer.h"

#ifdef __cplusplus
extern "C" {
#endif

SyanBufferInitStatus syan_global_buffer_init();

SyanEvent* syan_global_buffer_acquire_event_slot();

SyanBufferPagePtr syan_global_buffer_get_front_page();

void syan_global_buffer_release_front_page();

#ifdef __cplusplus
}
#endif

#endif
