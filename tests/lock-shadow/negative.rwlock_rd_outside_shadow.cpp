#include <cxxsync/mutex.hpp>
#include <cxxsync/rwlock.hpp>
#include <cxxsync/thread.hpp>

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
    RWLockWriteGuard g3(&rwl);
  });
  Thread t2([&]() {
    MutexGuard g1(&m);
    RWLockWriteGuard g3(&rwl);
  });
  Thread t3([&]() {
    MutexGuard g1(&m);
    RWLockWriteGuard g3(&rwl);
  });
  { RWLockReadGuard g3(&rwl); }
  return 0;
}
