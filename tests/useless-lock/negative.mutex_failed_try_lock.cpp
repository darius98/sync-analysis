#include <cxx_sync/mutex.hpp>
#include <cxx_sync/thread.hpp>

using sync::Mutex;
using sync::Thread;

int main() {
  Mutex m;
  m.lock();
  Thread t([&]() {
    m.try_lock();
  });
  t.join();
  m.unlock();
  return 0;
}
