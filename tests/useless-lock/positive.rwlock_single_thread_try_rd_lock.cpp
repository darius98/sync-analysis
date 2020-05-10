#include <cxx_sync/rwlock.hpp>

#include <cassert>

using sync::RWLock;

int main() {
  RWLock m;
  for (int i = 0; i < 100; i++) {
    const auto first_try_lock_result = m.try_rd_lock();
    assert(first_try_lock_result);
    const auto second_try_lock_result = m.try_rd_lock();
    assert(second_try_lock_result);
    m.rd_unlock();
  }
  return 0;
}
