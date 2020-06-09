#ifndef SYNC_ANALYSIS_H_
#define SYNC_ANALYSIS_H_

#ifdef __cplusplus
extern "C" {
#endif

void* syan_allocate_event(int event_type);
void syan_capture_allocated_event(void* event, void* addr);
void syan_capture_event(int event_type, void* addr);

#ifdef __cplusplus
}
#endif

#define SYNC_ANALYSIS_THREAD_CREATE_EVENT_TYPE 17

#endif
