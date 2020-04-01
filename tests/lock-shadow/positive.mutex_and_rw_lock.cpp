#include <cxx_sync/mutex.hpp>
#include <cxx_sync/rwlock.hpp>
#include <cxx_sync/thread.hpp>

using sync::Mutex;
using sync::MutexGuard;
using sync::RWLock;
using sync::RWLockReadGuard;
using sync::RWLockWriteGuard;
using sync::Thread;

int main() {
  Mutex m;
  RWLock rwl;
  Thread t1([&]() {
    MutexGuard g1(&m);
    RWLockWriteGuard g2(&rwl);
  });
  Thread t2([&]() {
    MutexGuard g1(&m);
    RWLockWriteGuard g2(&rwl);
  });
  Thread t3([&]() {
    MutexGuard g1(&m);
    RWLockWriteGuard g2(&rwl);
  });
  MutexGuard g1(&m);
  RWLockReadGuard g2(&rwl);
  return 0;
}
