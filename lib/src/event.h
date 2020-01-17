#ifndef SYNC_ANALYSIS_LIB_SRC_EVENT_H_
#define SYNC_ANALYSIS_LIB_SRC_EVENT_H_

#include <stdatomic.h>
#include <stdint.h>

#define SYAN_EVENT_SIGNATURE 666013

#ifdef __cplusplus
extern "C" {
#endif

// total size = 128B
typedef struct {
  atomic_int_fast32_t signature; // 4B
  int32_t event_type;            // 4B
  int64_t timestamp;             // 8B
  intptr_t thread_id;            // 8B
  intptr_t addr;                 // 8B
  intptr_t backtrace[12];        // 96B
} Event;

#ifdef __cplusplus
}
#endif

#endif
