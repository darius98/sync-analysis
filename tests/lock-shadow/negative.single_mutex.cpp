#include <cxxsync/mutex.hpp>
#include <cxxsync/thread.hpp>

using sync::Mutex;
using sync::MutexGuard;
using sync::Thread;

int main() {
  Mutex m1;
  Thread t1([&]() {
    MutexGuard g1(&m1);
  });
  Thread t2([&]() {
    MutexGuard g2(&m1);
  });
  Thread t3([&]() {
    MutexGuard g3(&m1);
  });
  MutexGuard g_main(&m1);
  return 0;
}
