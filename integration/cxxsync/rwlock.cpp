#include "cxxsync/rwlock.hpp"

#include "../shared/events.h"
#include "exception.hpp"

namespace sync {

RWLock::RWLock() {
  int status = pthread_rwlock_init(&pt_rwlock, nullptr);
  SyncException::throw_on_error("RWLock", "pthread_rwlock_init", status);
  syan_capture_event(SA_EV_RWLOCK_ON_CREATE, this);
}

RWLock::~RWLock() {
  pthread_rwlock_destroy(&pt_rwlock);
  syan_capture_event(SA_EV_RWLOCK_ON_DESTROY, this);
}

bool RWLock::try_rd_lock() noexcept {
  syan_capture_event(SA_EV_RWLOCK_ON_TRY_RD_LOCK, this);
  if (pthread_rwlock_tryrdlock(&pt_rwlock) == 0) {
    syan_capture_event(SA_EV_RWLOCK_AFTER_RD_LOCK, this);
    return true;
  }
  return false;
}

void RWLock::rd_lock() {
  syan_capture_event(SA_EV_RWLOCK_BEFORE_RD_LOCK, this);
  int status = pthread_rwlock_rdlock(&pt_rwlock);
  SyncException::throw_on_error("RWLock", "pthread_rwlock_rdlock", status);
  syan_capture_event(SA_EV_RWLOCK_AFTER_RD_LOCK, this);
}

void RWLock::rd_unlock() {
  syan_capture_event(SA_EV_RWLOCK_ON_UNLOCK, this);
  int status = pthread_rwlock_unlock(&pt_rwlock);
  SyncException::throw_on_error("RWLock", "pthread_rwlock_unlock", status);
}

bool RWLock::try_wr_lock() noexcept {
  syan_capture_event(SA_EV_RWLOCK_ON_TRY_WR_LOCK, this);
  if (pthread_rwlock_trywrlock(&pt_rwlock) == 0) {
    syan_capture_event(SA_EV_RWLOCK_AFTER_WR_LOCK, this);
    return true;
  }
  return false;
}

void RWLock::wr_lock() {
  syan_capture_event(SA_EV_RWLOCK_BEFORE_WR_LOCK, this);
  int status = pthread_rwlock_wrlock(&pt_rwlock);
  SyncException::throw_on_error("RWLock", "pthread_rwlock_wrlock", status);
  syan_capture_event(SA_EV_RWLOCK_AFTER_WR_LOCK, this);
}

void RWLock::wr_unlock() {
  syan_capture_event(SA_EV_RWLOCK_ON_UNLOCK, this);
  int status = pthread_rwlock_unlock(&pt_rwlock);
  SyncException::throw_on_error("RWLock", "pthread_rwlock_unlock", status);
}

RWLockReadGuard::RWLockReadGuard(RWLock* rwlock): rwlock(rwlock) {
  rwlock->rd_lock();
}

RWLockReadGuard::~RWLockReadGuard() {
  rwlock->rd_unlock();
}

RWLockWriteGuard::RWLockWriteGuard(RWLock* rwlock): rwlock(rwlock) {
  rwlock->wr_lock();
}

RWLockWriteGuard::~RWLockWriteGuard() {
  rwlock->wr_unlock();
}

}  // namespace sync
