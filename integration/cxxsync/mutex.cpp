#include "cxxsync/mutex.hpp"

#include "../shared/events.h"
#include "exception.hpp"

namespace sync {

Mutex::Mutex() {
  int status = pthread_mutex_init(&pt_mutex, nullptr);
  SyncException::throw_on_error("Mutex", "pthread_mutex_init", status);
  syan_capture_event(SA_EV_MUTEX_ON_CREATE, this);
}

Mutex::~Mutex() {
  pthread_mutex_destroy(&pt_mutex);
  syan_capture_event(SA_EV_MUTEX_ON_DESTROY, this);
}

bool Mutex::try_lock() noexcept {
  syan_capture_event(SA_EV_MUTEX_ON_TRY_LOCK, this);
  if (pthread_mutex_trylock(&pt_mutex) == 0) {
    syan_capture_event(SA_EV_MUTEX_AFTER_LOCK, this);
    return true;
  }
  return false;
}

void Mutex::lock() {
  syan_capture_event(SA_EV_MUTEX_BEFORE_LOCK, this);
  int status = pthread_mutex_lock(&pt_mutex);
  SyncException::throw_on_error("Mutex", "pthread_mutex_lock", status);
  syan_capture_event(SA_EV_MUTEX_AFTER_LOCK, this);
}

void Mutex::unlock() {
  syan_capture_event(SA_EV_MUTEX_ON_UNLOCK, this);
  int status = pthread_mutex_unlock(&pt_mutex);
  SyncException::throw_on_error("Mutex", "pthread_mutex_unlock", status);
}

MutexGuard::MutexGuard(Mutex* mtx): mtx(mtx) {
  mtx->lock();
}

MutexGuard::~MutexGuard() {
  mtx->unlock();
}

}  // namespace sync
