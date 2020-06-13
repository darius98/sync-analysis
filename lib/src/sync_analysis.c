#include <sync_analysis.h>

#include <stdatomic.h>

#include <pthread.h>

#include "backtrace.h"
#include "event.h"
#include "event_time.h"
#include "global_buffer.h"
#include "init.h"

__attribute__((constructor)) static void syan_init_call(int argc, char** argv) {
  syan_init(argc, argv);
}

__attribute__((destructor)) static void syan_shutdown_call() {
  syan_shutdown();
}

void* syan_initialize_event(int event_type) {
  SyanEvent* event = syan_global_buffer_acquire_event_slot();
  event->event_type = event_type;
  event->timestamp = syan_event_time_now();
  event->thread_id = (intptr_t)pthread_self();
  syan_get_backtrace(event->backtrace);
  return event;
}

void syan_finalize_event(void* event, void* addr) {
  ((SyanEvent*)event)->addr = (intptr_t)addr;
  atomic_store(&((SyanEvent*)event)->signature, SYAN_EVENT_SIGNATURE);
}
