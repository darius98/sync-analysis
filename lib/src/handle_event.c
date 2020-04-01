#include "handle_event.h"

#include <stdatomic.h>

#include <pthread.h>

#include "backtrace.h"
#include "event.h"
#include "event_time.h"
#include "global_buffer.h"

SyanEvent* syan_prepare_event(SyanEventType event_type) {
  SyanEvent* event = syan_global_buffer_acquire_event_slot();
  event->event_type = event_type;
  event->timestamp = syan_event_time_now();
  event->thread_id = (intptr_t)pthread_self();
  syan_get_backtrace(event->backtrace);
  return event;
}

void syan_finish_event(SyanEvent* event, void* addr) {
  event->addr = (intptr_t)addr;
  atomic_store(&event->signature, SYAN_EVENT_SIGNATURE);
}

void syan_handle_event(SyanEventType event_type, void* addr) {
  syan_finish_event(syan_prepare_event(event_type), addr);
}
