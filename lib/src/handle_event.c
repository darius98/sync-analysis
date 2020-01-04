#include "handle_event.h"

#include <stdatomic.h>

#include <pthread.h>

#include "backtrace.h"
#include "event.h"
#include "event_time.h"
#include "global_buffer.h"

void syan_handle_event(EventType event_type, void* addr) {
  Event* event = syan_global_buffer_acquire_event_slot();
  event->event_type = event_type;
  event->timestamp = syan_event_time_now();
  event->thread_id = (intptr_t)pthread_self();
  event->addr = (intptr_t)addr;
  syan_get_backtrace(event->backtrace);
  atomic_store_explicit(&event->signature, SYAN_EVENT_SIGNATURE,
                        memory_order_release);
}
