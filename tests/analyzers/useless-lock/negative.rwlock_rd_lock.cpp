#include <cxxsync/rwlock.hpp>
#include <cxxsync/thread.hpp>

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
