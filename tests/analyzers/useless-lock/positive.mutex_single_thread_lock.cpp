#include <cxxsync/mutex.hpp>

#include <cassert>

using sync::Mutex;

int main() {
  Mutex m;
  for (int i = 0; i < 100; i++) {
    m.lock();
    const auto try_lock_result = m.try_lock();
    assert(!try_lock_result);
    m.unlock();
  }
  return 0;
}
