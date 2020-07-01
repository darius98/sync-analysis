#include "cxxsync/recursive_mutex.hpp"

#include "../shared/events.h"
#include "exception.hpp"

namespace sync {

RecursiveMutex::RecursiveMutex() {
  pthread_mutexattr_t attr{};
  int status = pthread_mutexattr_init(&attr);
  SyncException::throw_on_error("RecursiveMutex", "pthread_mutexattr_init",
                                status);
  status = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
  SyncException::throw_on_error("RecursiveMutex", "pthread_mutexattr_settype",
                                status);
  status = pthread_mutex_init(&pt_mutex, &attr);
  SyncException::throw_on_error("RecursiveMutex", "pthread_mutex_init", status);
  status = pthread_mutexattr_destroy(&attr);
  SyncException::throw_on_error("RecursiveMutex", "pthread_mutexattr_destroy",
                                status);
  syan_capture_event(SA_EV_REC_MUTEX_ON_CREATE, this);
}

RecursiveMutex::~RecursiveMutex() {
  pthread_mutex_destroy(&pt_mutex);
  syan_capture_event(SA_EV_MUTEX_ON_DESTROY, this);
}

bool RecursiveMutex::try_lock() noexcept {
  syan_capture_event(SA_EV_MUTEX_ON_TRY_LOCK, this);
  if (pthread_mutex_trylock(&pt_mutex) == 0) {
    syan_capture_event(SA_EV_MUTEX_AFTER_LOCK, this);
    return true;
  }
  return false;
}

void RecursiveMutex::lock() {
  syan_capture_event(SA_EV_MUTEX_BEFORE_LOCK, this);
  int status = pthread_mutex_lock(&pt_mutex);
  SyncException::throw_on_error("RecursiveMutex", "pthread_mutex_lock", status);
  syan_capture_event(SA_EV_MUTEX_AFTER_LOCK, this);
}

void RecursiveMutex::unlock() {
  syan_capture_event(SA_EV_MUTEX_ON_UNLOCK, this);
  int status = pthread_mutex_unlock(&pt_mutex);
  SyncException::throw_on_error("RecursiveMutex", "pthread_mutex_unlock",
                                status);
}

RecursiveMutexGuard::RecursiveMutexGuard(RecursiveMutex* mtx): mtx(mtx) {
  mtx->lock();
}

RecursiveMutexGuard::~RecursiveMutexGuard() {
  mtx->unlock();
}

}  // namespace sync
