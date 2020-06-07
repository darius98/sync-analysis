#include <cxx_sync/rwlock.hpp>
#include <cxx_sync/thread.hpp>

using sync::RWLock;
using sync::Thread;

int main() {
  RWLock m;
  Thread t([&]() {
    m.rd_lock();
    m.rd_unlock();
  });
  m.wr_lock();
  m.wr_unlock();
  return 0;
}