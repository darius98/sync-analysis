#include "handle_event.h"

// Thread

void syan_thread_on_create(void* addr) {
  handle_event(SA_EV_THREAD_ON_CREATE, addr);
}

void syan_thread_on_join(void* addr) {
  handle_event(SA_EV_THREAD_ON_JOIN, addr);
}

void syan_thread_on_detach(void* addr) {
  handle_event(SA_EV_THREAD_ON_DETACH, addr);
}

// Mutex

void syan_mutex_on_create(void* addr) {
  handle_event(SA_EV_MUTEX_ON_CREATE, addr);
}

void syan_mutex_on_try_lock(void* addr) {
  handle_event(SA_EV_MUTEX_ON_TRY_LOCK, addr);
}

void syan_mutex_before_lock(void* addr) {
  handle_event(SA_EV_MUTEX_BEFORE_LOCK, addr);
}

void syan_mutex_after_lock(void* addr) {
  handle_event(SA_EV_MUTEX_AFTER_LOCK, addr);
}

void syan_mutex_on_unlock(void* addr) {
  handle_event(SA_EV_MUTEX_ON_UNLOCK, addr);
}

void syan_mutex_on_destroy(void* addr) {
  handle_event(SA_EV_MUTEX_ON_DESTROY, addr);
}

// Recursive mutex

void syan_rec_mutex_on_create(void* addr) {
    handle_event(SA_EV_REC_MUTEX_ON_CREATE, addr);
}

void syan_rec_mutex_on_try_lock(void* addr) {
    handle_event(SA_EV_REC_MUTEX_ON_TRY_LOCK, addr);
}

void syan_rec_mutex_before_lock(void* addr) {
    handle_event(SA_EV_REC_MUTEX_BEFORE_LOCK, addr);
}

void syan_rec_mutex_after_lock(void* addr) {
    handle_event(SA_EV_REC_MUTEX_AFTER_LOCK, addr);
}

void syan_rec_mutex_on_unlock(void* addr) {
    handle_event(SA_EV_REC_MUTEX_ON_UNLOCK, addr);
}

void syan_rec_mutex_on_destroy(void* addr) {
    handle_event(SA_EV_REC_MUTEX_ON_DESTROY, addr);
}

// Read-write lock

void syan_rwlock_on_create(void* addr) {
  handle_event(SA_EV_RWLOCK_ON_CREATE, addr);
}

void syan_rwlock_on_try_rd_lock(void* addr) {
  handle_event(SA_EV_RWLOCK_ON_TRY_RD_LOCK, addr);
}

void syan_rwlock_before_rd_lock(void* addr) {
  handle_event(SA_EV_RWLOCK_BEFORE_RD_LOCK, addr);
}

void syan_rwlock_after_rd_lock(void* addr) {
  handle_event(SA_EV_RWLOCK_AFTER_RD_LOCK, addr);
}

void syan_rwlock_on_rd_unlock(void* addr) {
  handle_event(SA_EV_RWLOCK_ON_RD_UNLOCK, addr);
}

void syan_rwlock_on_try_wr_lock(void* addr) {
  handle_event(SA_EV_RWLOCK_ON_TRY_WR_LOCK, addr);
}

void syan_rwlock_before_wr_lock(void* addr) {
  handle_event(SA_EV_RWLOCK_BEFORE_WR_LOCK, addr);
}

void syan_rwlock_after_wr_lock(void* addr) {
  handle_event(SA_EV_RWLOCK_AFTER_WR_LOCK, addr);
}

void syan_rwlock_on_wr_unlock(void* addr) {
  handle_event(SA_EV_RWLOCK_ON_WR_UNLOCK, addr);
}

void syan_rwlock_on_destroy(void* addr) {
  handle_event(SA_EV_RWLOCK_ON_DESTROY, addr);
}
