#ifndef SYNC_ANALYSIS_INTEGRATION_CXXSYNC_INCLUDE_CXXSYNC_MUTEX_HPP_
#define SYNC_ANALYSIS_INTEGRATION_CXXSYNC_INCLUDE_CXXSYNC_MUTEX_HPP_

#include <pthread.h>

namespace sync {

class Mutex {
public:
  Mutex();

  ~Mutex();

  bool try_lock() noexcept;

  void lock();

  void unlock();

private:
  pthread_mutex_t pt_mutex;
};

class MutexGuard {
public:
  explicit MutexGuard(Mutex* mtx);

  ~MutexGuard();

private:
  Mutex* mtx;
};

}  // namespace sync

#endif
