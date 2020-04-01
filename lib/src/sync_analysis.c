#include "handle_event.h"
#include "init.h"

__attribute__((constructor)) static void syan_init_call(int argc, char** argv) {
  syan_init(argc, argv);
}

__attribute__((destructor)) static void syan_shutdown_call() {
  syan_shutdown();
}

// Thread

void* syan_thread_on_create_init() {
  return syan_prepare_event(SA_EV_THREAD_ON_CREATE);
}

void syan_thread_on_create_finalize(void* event, void* addr) {
  syan_finish_event((SyanEvent*)event, addr);
}

void syan_thread_on_create(void* addr) {
  syan_finish_event((SyanEvent*)syan_thread_on_create_init(), addr);
}

void syan_thread_on_join(void* addr) {
  syan_handle_event(SA_EV_THREAD_ON_JOIN, addr);
}

void syan_thread_on_detach(void* addr) {
  syan_handle_event(SA_EV_THREAD_ON_DETACH, addr);
}

// Mutex

void syan_mutex_on_create(void* addr) {
  syan_handle_event(SA_EV_MUTEX_ON_CREATE, addr);
}

void syan_mutex_on_try_lock(void* addr) {
  syan_handle_event(SA_EV_MUTEX_ON_TRY_LOCK, addr);
}

void syan_mutex_before_lock(void* addr) {
  syan_handle_event(SA_EV_MUTEX_BEFORE_LOCK, addr);
}

void syan_mutex_after_lock(void* addr) {
  syan_handle_event(SA_EV_MUTEX_AFTER_LOCK, addr);
}

void syan_mutex_on_unlock(void* addr) {
  syan_handle_event(SA_EV_MUTEX_ON_UNLOCK, addr);
}

void syan_mutex_on_destroy(void* addr) {
  syan_handle_event(SA_EV_MUTEX_ON_DESTROY, addr);
}

// Recursive mutex

void syan_rec_mutex_on_create(void* addr) {
  syan_handle_event(SA_EV_REC_MUTEX_ON_CREATE, addr);
}

void syan_rec_mutex_on_try_lock(void* addr) {
  syan_handle_event(SA_EV_REC_MUTEX_ON_TRY_LOCK, addr);
}

void syan_rec_mutex_before_lock(void* addr) {
  syan_handle_event(SA_EV_REC_MUTEX_BEFORE_LOCK, addr);
}

void syan_rec_mutex_after_lock(void* addr) {
  syan_handle_event(SA_EV_REC_MUTEX_AFTER_LOCK, addr);
}

void syan_rec_mutex_on_unlock(void* addr) {
  syan_handle_event(SA_EV_REC_MUTEX_ON_UNLOCK, addr);
}

void syan_rec_mutex_on_destroy(void* addr) {
  syan_handle_event(SA_EV_REC_MUTEX_ON_DESTROY, addr);
}

// Read-write lock

void syan_rwlock_on_create(void* addr) {
  syan_handle_event(SA_EV_RWLOCK_ON_CREATE, addr);
}

void syan_rwlock_on_try_rd_lock(void* addr) {
  syan_handle_event(SA_EV_RWLOCK_ON_TRY_RD_LOCK, addr);
}

void syan_rwlock_before_rd_lock(void* addr) {
  syan_handle_event(SA_EV_RWLOCK_BEFORE_RD_LOCK, addr);
}

void syan_rwlock_after_rd_lock(void* addr) {
  syan_handle_event(SA_EV_RWLOCK_AFTER_RD_LOCK, addr);
}

void syan_rwlock_on_rd_unlock(void* addr) {
  syan_handle_event(SA_EV_RWLOCK_ON_RD_UNLOCK, addr);
}

void syan_rwlock_on_try_wr_lock(void* addr) {
  syan_handle_event(SA_EV_RWLOCK_ON_TRY_WR_LOCK, addr);
}

void syan_rwlock_before_wr_lock(void* addr) {
  syan_handle_event(SA_EV_RWLOCK_BEFORE_WR_LOCK, addr);
}

void syan_rwlock_after_wr_lock(void* addr) {
  syan_handle_event(SA_EV_RWLOCK_AFTER_WR_LOCK, addr);
}

void syan_rwlock_on_wr_unlock(void* addr) {
  syan_handle_event(SA_EV_RWLOCK_ON_WR_UNLOCK, addr);
}

void syan_rwlock_on_destroy(void* addr) {
  syan_handle_event(SA_EV_RWLOCK_ON_DESTROY, addr);
}
