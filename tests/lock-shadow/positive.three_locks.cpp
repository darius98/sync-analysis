#include <cxxsync/mutex.hpp>
#include <cxxsync/recursive_mutex.hpp>
#include <cxxsync/rwlock.hpp>
#include <cxxsync/thread.hpp>

using sync::Mutex;
using sync::MutexGuard;
using sync::RecursiveMutex;
using sync::RecursiveMutexGuard;
using sync::RWLock;
using sync::RWLockWriteGuard;
using sync::Thread;

int main() {
  Mutex m;
  RecursiveMutex rm;
  RWLock rwl;
  Thread t1([&]() {
    MutexGuard g1(&m);
    RecursiveMutexGuard g2(&rm);
    RecursiveMutexGuard g2_2(&rm);
    RWLockWriteGuard g3(&rwl);
  });
  Thread t2([&]() {
    MutexGuard g1(&m);
    RecursiveMutexGuard g2(&rm);
    RecursiveMutexGuard g2_2(&rm);
    RWLockWriteGuard g3(&rwl);
  });
  Thread t3([&]() {
    MutexGuard g1(&m);
    RecursiveMutexGuard g2(&rm);
    RecursiveMutexGuard g2_2(&rm);
    RWLockWriteGuard g3(&rwl);
  });
  MutexGuard g1(&m);
  RecursiveMutexGuard g2(&rm);
  RecursiveMutexGuard g2_2(&rm);
  RWLockWriteGuard g3(&rwl);
  return 0;
}
