#ifndef SYNC_ANALYSIS_H_
#define SYNC_ANALYSIS_H_

#ifdef __cplusplus
extern "C" {
#endif

void* syan_initialize_event(int event_type);
void syan_finalize_event(void* event, void* addr);

void syan_capture_event(int event_type, void* addr);

enum {
  SA_EV_CREATE = 1 << 28,
  SA_EV_DESTROY = 1 << 29,
  SA_EV_THREAD = 1 << 30,
  SA_EV_THREAD_ON_CREATE = SA_EV_THREAD | SA_EV_CREATE,
};

#ifdef __cplusplus
}
#endif

#endif
