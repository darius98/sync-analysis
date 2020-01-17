#ifndef SYNC_ANALYSIS_LIB_SRC_EVENT_TIME_H_
#define SYNC_ANALYSIS_LIB_SRC_EVENT_TIME_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int syan_event_time_init();

int64_t syan_event_time_now();

#ifdef __cplusplus
}
#endif


#endif
