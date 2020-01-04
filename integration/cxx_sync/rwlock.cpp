#include "rwlock.hpp"

#include <cassert>

#include <sync_analysis.h>

#include "exception.hpp"

namespace sync {

RWLock::RWLock() {
  pthread_rwlockattr_t attr{};
  int status = pthread_rwlockattr_init(&attr);
  SyncException::throw_on_error("RWLock", "pthread_rwlockattr_init", status);
  status = pthread_rwlock_init(&pt_rwlock, &attr);
  SyncException::throw_on_error("RWLock", "pthread_rwlock_init", status);
  status = pthread_rwlockattr_destroy(&attr);
  SyncException::throw_on_error("RWLock", "pthread_rwlockattr_destroy", status);
  syan_rwlock_on_create(this);
}

RWLock::~RWLock() {
  int status = pthread_rwlock_destroy(&pt_rwlock);
  assert(status == 0);
  syan_rwlock_on_destroy(this);
}

bool RWLock::try_rd_lock() noexcept {
  syan_rwlock_on_try_rd_lock(this);
  return pthread_rwlock_tryrdlock(&pt_rwlock) == 0;
}

void RWLock::rd_lock() {
  syan_rwlock_before_rd_lock(this);
  int status = pthread_rwlock_rdlock(&pt_rwlock);
  SyncException::throw_on_error("RWLock", "pthread_rwlock_rdlock", status);
  syan_rwlock_after_rd_lock(this);
}

void RWLock::rd_unlock() {
  syan_rwlock_on_rd_unlock(this);
  int status = pthread_rwlock_unlock(&pt_rwlock);
  SyncException::throw_on_error("RWLock", "pthread_rwlock_unlock", status);
}

bool RWLock::try_wr_lock() noexcept {
  syan_rwlock_on_try_wr_lock(this);
  return pthread_rwlock_trywrlock(&pt_rwlock) == 0;
}

void RWLock::wr_lock() {
  syan_rwlock_before_wr_lock(this);
  int status = pthread_rwlock_wrlock(&pt_rwlock);
  SyncException::throw_on_error("RWLock", "pthread_rwlock_wrlock", status);
  syan_rwlock_after_wr_lock(this);
}

void RWLock::wr_unlock() {
  syan_rwlock_on_wr_unlock(this);
  int status = pthread_rwlock_unlock(&pt_rwlock);
  SyncException::throw_on_error("RWLock", "pthread_rwlock_unlock", status);
}

RWLockReadGuard::RWLockReadGuard(RWLock* rwlock) : rwlock(rwlock) {
  rwlock->rd_lock();
}

RWLockReadGuard::~RWLockReadGuard() {
  rwlock->rd_unlock();
}

RWLockWriteGuard::RWLockWriteGuard(RWLock* rwlock) : rwlock(rwlock) {
  rwlock->wr_lock();
}

RWLockWriteGuard::~RWLockWriteGuard() {
  rwlock->wr_unlock();
}

} // namespace sync
