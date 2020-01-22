#ifndef SYNC_ANALYSIS_LIB_SRC_HANDLE_EVENT_H_
#define SYNC_ANALYSIS_LIB_SRC_HANDLE_EVENT_H_

#include "event_type.h"

#ifdef __cplusplus
extern "C" {
#endif

void syan_handle_event(SyanEventType event_type, void* addr);

#ifdef __cplusplus
}
#endif

#endif
