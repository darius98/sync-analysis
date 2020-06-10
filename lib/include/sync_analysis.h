#ifndef SYNC_ANALYSIS_H_
#define SYNC_ANALYSIS_H_

#ifdef __cplusplus
extern "C" {
#endif

void* syan_initialize_event(int event_type);
void syan_finalize_event(void* event, void* addr);
void syan_capture_event(int event_type, void* addr);

#ifdef __cplusplus
}
#endif

#define SYAN_THREAD_CREATE_EVENT 17

#endif
