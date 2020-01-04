#include "mutex.hpp"

#include <cassert>

#include <sync_analysis.h>

#include "exception.hpp"

namespace sync {

Mutex::Mutex() {
  pthread_mutexattr_t attr{};
  int status = pthread_mutexattr_init(&attr);
  SyncException::throw_on_error("Mutex", "pthread_mutexattr_init", status);
  status = pthread_mutex_init(&pt_mutex, &attr);
  SyncException::throw_on_error("Mutex", "pthread_mutex_init", status);
  status = pthread_mutexattr_destroy(&attr);
  SyncException::throw_on_error("Mutex", "pthread_mutexattr_destroy", status);
  syan_mutex_on_create(this);
}

Mutex::~Mutex() {
  int status = pthread_mutex_destroy(&pt_mutex);
  assert(status == 0);
  syan_mutex_on_destroy(this);
}

bool Mutex::try_lock() noexcept {
  syan_mutex_on_try_lock(this);
  return pthread_mutex_trylock(&pt_mutex) == 0;
}

void Mutex::lock() {
  syan_mutex_before_lock(this);
  int status = pthread_mutex_lock(&pt_mutex);
  SyncException::throw_on_error("Mutex", "pthread_mutex_lock", status);
  syan_mutex_after_lock(this);
}

void Mutex::unlock() {
  syan_mutex_on_unlock(this);
  int status = pthread_mutex_unlock(&pt_mutex);
  SyncException::throw_on_error("Mutex", "pthread_mutex_unlock", status);
}

MutexGuard::MutexGuard(Mutex* mtx) : mtx(mtx) {
  mtx->lock();
}

MutexGuard::~MutexGuard() {
  mtx->unlock();
}

} // namespace sync
