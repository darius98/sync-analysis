#include <cxxsync/rwlock.hpp>

#include <cassert>

using sync::RWLock;

int main() {
  RWLock m;
  for (int i = 0; i < 100; i++) {
    m.wr_lock();
    m.wr_unlock();
  }
  return 0;
}
