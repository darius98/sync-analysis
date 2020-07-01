#include <cxxsync/rwlock.hpp>

using sync::RWLock;

int main() {
  RWLock m;
  for (int i = 0; i < 100; i++) {
    m.wr_lock();
    m.unlock();
  }
  return 0;
}
