#include "handle_event.h"

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

// Read-write lock

void syan_rwlock_on_create(void* addr) {
  handle_event(SA_EV_RWLOCK_ON_CREATE, addr);
}

void syan_rwlock_on_try_lock(void* addr) {
  handle_event(SA_EV_RWLOCK_ON_TRY_LOCK, addr);
}

void syan_rwlock_before_lock(void* addr) {
  handle_event(SA_EV_RWLOCK_BEFORE_LOCK, addr);
}

void syan_rwlock_after_lock(void* addr) {
  handle_event(SA_EV_RWLOCK_AFTER_LOCK, addr);
}

void syan_rwlock_on_unlock(void* addr) {
  handle_event(SA_EV_RWLOCK_ON_UNLOCK, addr);
}

void syan_rwlock_on_w_try_lock(void* addr) {
  handle_event(SA_EV_RWLOCK_ON_W_TRY_LOCK, addr);
}

void syan_rwlock_before_w_lock(void* addr) {
  handle_event(SA_EV_RWLOCK_BEFORE_W_LOCK, addr);
}

void syan_rwlock_after_w_lock(void* addr) {
  handle_event(SA_EV_RWLOCK_AFTER_W_LOCK, addr);
}

void syan_rwlock_on_w_unlock(void* addr) {
  handle_event(SA_EV_RWLOCK_ON_W_UNLOCK, addr);
}

void syan_rwlock_on_destroy(void* addr) {
  handle_event(SA_EV_RWLOCK_ON_DESTROY, addr);
}
