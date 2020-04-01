#include <cxx_sync/mutex.hpp>
#include <cxx_sync/thread.hpp>

using sync::Mutex;
using sync::MutexGuard;
using sync::Thread;

int main() {
  Mutex m1, m2;
  Thread t1([&]() {
    MutexGuard g1(&m1);
    MutexGuard g2(&m2);
  });
  Thread t2([&]() {
    MutexGuard g1(&m1);
    MutexGuard g2(&m2);
  });
  Thread t3([&]() {
    MutexGuard g2(&m2);
  });
  MutexGuard g_main1(&m1);
  MutexGuard g_main2(&m2);
  return 0;
}
