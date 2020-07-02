#include <stdio.h>

#define __USE_GNU
#include <dlfcn.h>
#include <pthread.h>

#include <sync_analysis.h>

#include "shared/events.h"

#define SYAN_SHIM(NAME, ARGS)                                                  \
  typedef int(*NAME##_t) ARGS;                                                 \
  static NAME##_t syan_shimmed_##NAME = NULL;                                  \
  static void syan_load_shimmed_##NAME() {                                     \
    syan_shimmed_##NAME = (NAME##_t)dlsym(RTLD_NEXT, #NAME);                   \
    if (syan_shimmed_##NAME == NULL) {                                         \
      fprintf(stderr,                                                          \
              "Syan Pthread shim warning: Failed to load symbol " #NAME        \
              ": %s\n",                                                        \
              dlerror());                                                      \
    }                                                                          \
  }                                                                            \
  int NAME ARGS

SYAN_SHIM(pthread_create, (pthread_t * id, const pthread_attr_t* attr,
                           void* (*func)(void*), void* arg)) {
  void* event = syan_initialize_event(SA_EV_THREAD_ON_CREATE);
  int result = syan_shimmed_pthread_create(id, attr, func, arg);
  syan_finalize_event(event, (void*)*id);
  return result;
}

SYAN_SHIM(pthread_join, (pthread_t id, void** ret)) {
  int result = syan_shimmed_pthread_join(id, ret);
  if (result == 0) {
    syan_capture_event(SA_EV_THREAD_ON_JOIN, (void*)id);
  }
  return result;
}

SYAN_SHIM(pthread_detach, (pthread_t id)) {
  int result = syan_shimmed_pthread_detach(id);
  if (result == 0) {
    syan_capture_event(SA_EV_THREAD_ON_DETACH, (void*)id);
  }
  return result;
}

SYAN_SHIM(pthread_rwlock_init,
          (pthread_rwlock_t * rwlock, const pthread_rwlockattr_t* attr)) {
  int result = syan_shimmed_pthread_rwlock_init(rwlock, attr);
  if (result == 0) {
    syan_capture_event(SA_EV_RWLOCK_ON_CREATE, rwlock);
  }
  return result;
}

SYAN_SHIM(pthread_rwlock_destroy, (pthread_rwlock_t * rwlock)) {
  int result = syan_shimmed_pthread_rwlock_destroy(rwlock);
  if (result == 0) {
    syan_capture_event(SA_EV_RWLOCK_ON_DESTROY, rwlock);
  }
  return result;
}

SYAN_SHIM(pthread_rwlock_tryrdlock, (pthread_rwlock_t * rwlock)) {
  syan_capture_event(SA_EV_RWLOCK_ON_TRY_RD_LOCK, rwlock);
  int result = syan_shimmed_pthread_rwlock_tryrdlock(rwlock);
  if (result == 0) {
    syan_capture_event(SA_EV_RWLOCK_AFTER_RD_LOCK, rwlock);
  }
  return result;
}

SYAN_SHIM(pthread_rwlock_rdlock, (pthread_rwlock_t * rwlock)) {
  syan_capture_event(SA_EV_RWLOCK_BEFORE_RD_LOCK, rwlock);
  int result = syan_shimmed_pthread_rwlock_rdlock(rwlock);
  if (result == 0) {
    syan_capture_event(SA_EV_RWLOCK_AFTER_RD_LOCK, rwlock);
  }
  return result;
}

SYAN_SHIM(pthread_rwlock_trywrlock, (pthread_rwlock_t * rwlock)) {
  syan_capture_event(SA_EV_RWLOCK_ON_TRY_WR_LOCK, rwlock);
  int result = syan_shimmed_pthread_rwlock_trywrlock(rwlock);
  if (result == 0) {
    syan_capture_event(SA_EV_RWLOCK_AFTER_WR_LOCK, rwlock);
  }
  return result;
}

SYAN_SHIM(pthread_rwlock_wrlock, (pthread_rwlock_t * rwlock)) {
  syan_capture_event(SA_EV_RWLOCK_BEFORE_WR_LOCK, rwlock);
  int result = syan_shimmed_pthread_rwlock_wrlock(rwlock);
  if (result == 0) {
    syan_capture_event(SA_EV_RWLOCK_AFTER_WR_LOCK, rwlock);
  }
  return result;
}

SYAN_SHIM(pthread_rwlock_unlock, (pthread_rwlock_t * rwlock)) {
  // TODO: Find out how we can differentiate between a write and a read unlock.
  syan_capture_event(SA_EV_RWLOCK_ON_UNLOCK, rwlock);
  return syan_shimmed_pthread_rwlock_unlock(rwlock);
}

SYAN_SHIM(pthread_mutex_init,
          (pthread_mutex_t * mutex, const pthread_mutexattr_t* attr)) {
  int result = syan_shimmed_pthread_mutex_init(mutex, attr);
  int type;
  int gettype_result = pthread_mutexattr_gettype(attr, &type);
  if (gettype_result == 0 && type == PTHREAD_MUTEX_RECURSIVE && result == 0) {
    syan_capture_event(SA_EV_REC_MUTEX_ON_CREATE, mutex);
  } else if (result == 0) {
    syan_capture_event(SA_EV_MUTEX_ON_CREATE, mutex);
  }
  return result;
}

SYAN_SHIM(pthread_mutex_destroy, (pthread_mutex_t * mutex)) {
  int result = syan_shimmed_pthread_mutex_destroy(mutex);
  if (result == 0) {
    syan_capture_event(SA_EV_MUTEX_ON_DESTROY, mutex);
  }
  return result;
}

SYAN_SHIM(pthread_mutex_trylock, (pthread_mutex_t * mutex)) {
  syan_capture_event(SA_EV_MUTEX_ON_TRY_LOCK, mutex);
  int result = syan_shimmed_pthread_mutex_trylock(mutex);
  if (result == 0) {
    syan_capture_event(SA_EV_MUTEX_AFTER_LOCK, mutex);
  }
  return result;
}

SYAN_SHIM(pthread_mutex_lock, (pthread_mutex_t * mutex)) {
  syan_capture_event(SA_EV_MUTEX_BEFORE_LOCK, mutex);
  int result = syan_shimmed_pthread_mutex_lock(mutex);
  if (result == 0) {
    syan_capture_event(SA_EV_MUTEX_AFTER_LOCK, mutex);
  }
  return result;
}

SYAN_SHIM(pthread_mutex_unlock, (pthread_mutex_t * mutex)) {
  syan_capture_event(SA_EV_MUTEX_ON_UNLOCK, mutex);
  return syan_shimmed_pthread_mutex_unlock(mutex);
}

__attribute__((__constructor__)) static void syan_shim_init() {
  syan_load_shimmed_pthread_create();
  syan_load_shimmed_pthread_join();
  syan_load_shimmed_pthread_detach();
  syan_load_shimmed_pthread_rwlock_init();
  syan_load_shimmed_pthread_rwlock_destroy();
  syan_load_shimmed_pthread_rwlock_tryrdlock();
  syan_load_shimmed_pthread_rwlock_rdlock();
  syan_load_shimmed_pthread_rwlock_trywrlock();
  syan_load_shimmed_pthread_rwlock_wrlock();
  syan_load_shimmed_pthread_rwlock_unlock();
  syan_load_shimmed_pthread_mutex_init();
  syan_load_shimmed_pthread_mutex_destroy();
  syan_load_shimmed_pthread_mutex_trylock();
  syan_load_shimmed_pthread_mutex_lock();
  syan_load_shimmed_pthread_mutex_unlock();
}
