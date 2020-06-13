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

enum {
  SA_EV_THREAD = 1u << 26,
  SA_EV_JOIN = 1u << 27,
  SA_EV_DETACH = 1u << 28,
  SA_EV_CREATE = 1u << 29,
  SA_EV_DESTROY = 1u << 30,
};

#endif
