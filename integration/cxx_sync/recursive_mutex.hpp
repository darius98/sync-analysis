#ifndef SYNC_ANALYSIS_INTEGRATION_CXX_SYNC_RECURSIVE_MUTEX_HPP_
#define SYNC_ANALYSIS_INTEGRATION_CXX_SYNC_RECURSIVE_MUTEX_HPP_

#include <pthread.h>

namespace sync {

class RecursiveMutex {
public:
  RecursiveMutex();

  ~RecursiveMutex();

  bool try_lock() noexcept;

  void lock();

  void unlock();

private:
  pthread_mutex_t pt_mutex;
};

class RecursiveMutexGuard {
public:
  explicit RecursiveMutexGuard(RecursiveMutex* mtx);

  ~RecursiveMutexGuard();

private:
  RecursiveMutex* mtx;
};

} // namespace sync

#endif
