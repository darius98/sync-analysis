#include <cxxsync/mutex.hpp>

#include <cassert>

using sync::Mutex;

int main() {
  Mutex m;
  for (int i = 0; i < 100; i++) {
    const auto first_try_lock_result = m.try_lock();
    assert(first_try_lock_result);
    const auto second_try_lock_result = m.try_lock();
    assert(!second_try_lock_result);
    m.unlock();
  }
  return 0;
}
