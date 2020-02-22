#ifndef SYNC_ANALYSIS_INTEGRATION_CXX_SYNC_RWLOCK_HPP_
#define SYNC_ANALYSIS_INTEGRATION_CXX_SYNC_RWLOCK_HPP_

#include <pthread.h>

namespace sync {

class RWLock {
public:
  RWLock();

  ~RWLock();

  bool try_rd_lock() noexcept;

  void rd_lock();

  void rd_unlock();

  bool try_wr_lock() noexcept;

  void wr_lock();

  void wr_unlock();

private:
  pthread_rwlock_t pt_rwlock;
};

class RWLockReadGuard {
public:
  explicit RWLockReadGuard(RWLock* rwlock);

  ~RWLockReadGuard();

private:
  RWLock* rwlock;
};

class RWLockWriteGuard {
public:
  explicit RWLockWriteGuard(RWLock* rwlock);

  ~RWLockWriteGuard();

private:
  RWLock* rwlock;
};

}  // namespace sync

#endif
