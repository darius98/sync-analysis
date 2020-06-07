#include "cxxsync/recursive_mutex.hpp"

#include <sync_analysis.h>

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
  syan_rec_mutex_on_create(this);
}

RecursiveMutex::~RecursiveMutex() {
  pthread_mutex_destroy(&pt_mutex);
  syan_rec_mutex_on_destroy(this);
}

bool RecursiveMutex::try_lock() noexcept {
  syan_rec_mutex_on_try_lock(this);
  if (pthread_mutex_trylock(&pt_mutex) == 0) {
    syan_rec_mutex_after_lock(this);
    return true;
  }
  return false;
}

void RecursiveMutex::lock() {
  syan_rec_mutex_before_lock(this);
  int status = pthread_mutex_lock(&pt_mutex);
  SyncException::throw_on_error("RecursiveMutex", "pthread_mutex_lock", status);
  syan_rec_mutex_after_lock(this);
}

void RecursiveMutex::unlock() {
  syan_rec_mutex_on_unlock(this);
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
