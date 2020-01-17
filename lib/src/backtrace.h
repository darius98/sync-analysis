#ifndef SYNC_ANALYSIS_LIB_SRC_BACKTRACE_H_
#define SYNC_ANALYSIS_LIB_SRC_BACKTRACE_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void syan_get_backtrace(intptr_t* backtrace);

#ifdef __cplusplus
}
#endif

#endif
