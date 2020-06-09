#include <sync_analysis.h>

#include <stdatomic.h>

#include <pthread.h>

#include "init.h"

#include "event.h"
#include "event_type.h"

#include "backtrace.h"
#include "event_time.h"
#include "global_buffer.h"

__attribute__((constructor)) static void syan_init_call(int argc, char** argv) {
  syan_init(argc, argv);
}

__attribute__((destructor)) static void syan_shutdown_call() {
  syan_shutdown();
}

void* syan_allocate_event(int event_type) {
  SyanEvent* event = syan_global_buffer_acquire_event_slot();
  event->event_type = event_type;
  event->timestamp = syan_event_time_now();
  event->thread_id = (intptr_t)pthread_self();
  syan_get_backtrace(event->backtrace);
  return event;
}

void syan_capture_allocated_event(void* event, void* addr) {
  ((SyanEvent*)event)->addr = (intptr_t)addr;
  atomic_store(&((SyanEvent*)event)->signature, SYAN_EVENT_SIGNATURE);
}

void syan_capture_event(int event_type, void* addr) {
  syan_capture_allocated_event(syan_allocate_event(event_type), addr);
}

// Thread

void* syan_thread_on_create_init() {
  return syan_allocate_event(SA_EV_THREAD_ON_CREATE);
}

void syan_thread_on_create_finalize(void* event, void* addr) {
  syan_capture_allocated_event(event, addr);
}

void syan_thread_on_create(void* addr) {
  syan_capture_event(SA_EV_THREAD_ON_CREATE, addr);
}

void syan_thread_on_join(void* addr) {
  syan_capture_event(SA_EV_THREAD_ON_JOIN, addr);
}

void syan_thread_on_detach(void* addr) {
  syan_capture_event(SA_EV_THREAD_ON_DETACH, addr);
}

// Mutex

void syan_mutex_on_create(void* addr) {
  syan_capture_event(SA_EV_MUTEX_ON_CREATE, addr);
}

void syan_mutex_on_try_lock(void* addr) {
  syan_capture_event(SA_EV_MUTEX_ON_TRY_LOCK, addr);
}

void syan_mutex_before_lock(void* addr) {
  syan_capture_event(SA_EV_MUTEX_BEFORE_LOCK, addr);
}

void syan_mutex_after_lock(void* addr) {
  syan_capture_event(SA_EV_MUTEX_AFTER_LOCK, addr);
}

void syan_mutex_on_unlock(void* addr) {
  syan_capture_event(SA_EV_MUTEX_ON_UNLOCK, addr);
}

void syan_mutex_on_destroy(void* addr) {
  syan_capture_event(SA_EV_MUTEX_ON_DESTROY, addr);
}

// Recursive mutex

void syan_rec_mutex_on_create(void* addr) {
  syan_capture_event(SA_EV_REC_MUTEX_ON_CREATE, addr);
}

void syan_rec_mutex_on_try_lock(void* addr) {
  syan_capture_event(SA_EV_REC_MUTEX_ON_TRY_LOCK, addr);
}

void syan_rec_mutex_before_lock(void* addr) {
  syan_capture_event(SA_EV_REC_MUTEX_BEFORE_LOCK, addr);
}

void syan_rec_mutex_after_lock(void* addr) {
  syan_capture_event(SA_EV_REC_MUTEX_AFTER_LOCK, addr);
}

void syan_rec_mutex_on_unlock(void* addr) {
  syan_capture_event(SA_EV_REC_MUTEX_ON_UNLOCK, addr);
}

void syan_rec_mutex_on_destroy(void* addr) {
  syan_capture_event(SA_EV_REC_MUTEX_ON_DESTROY, addr);
}

// Read-write lock

void syan_rwlock_on_create(void* addr) {
  syan_capture_event(SA_EV_RWLOCK_ON_CREATE, addr);
}

void syan_rwlock_on_try_rd_lock(void* addr) {
  syan_capture_event(SA_EV_RWLOCK_ON_TRY_RD_LOCK, addr);
}

void syan_rwlock_before_rd_lock(void* addr) {
  syan_capture_event(SA_EV_RWLOCK_BEFORE_RD_LOCK, addr);
}

void syan_rwlock_after_rd_lock(void* addr) {
  syan_capture_event(SA_EV_RWLOCK_AFTER_RD_LOCK, addr);
}

void syan_rwlock_on_rd_unlock(void* addr) {
  syan_capture_event(SA_EV_RWLOCK_ON_RD_UNLOCK, addr);
}

void syan_rwlock_on_try_wr_lock(void* addr) {
  syan_capture_event(SA_EV_RWLOCK_ON_TRY_WR_LOCK, addr);
}

void syan_rwlock_before_wr_lock(void* addr) {
  syan_capture_event(SA_EV_RWLOCK_BEFORE_WR_LOCK, addr);
}

void syan_rwlock_after_wr_lock(void* addr) {
  syan_capture_event(SA_EV_RWLOCK_AFTER_WR_LOCK, addr);
}

void syan_rwlock_on_wr_unlock(void* addr) {
  syan_capture_event(SA_EV_RWLOCK_ON_WR_UNLOCK, addr);
}

void syan_rwlock_on_destroy(void* addr) {
  syan_capture_event(SA_EV_RWLOCK_ON_DESTROY, addr);
}
