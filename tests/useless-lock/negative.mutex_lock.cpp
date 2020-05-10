#include <cxx_sync/mutex.hpp>
#include <cxx_sync/thread.hpp>

using sync::Mutex;
using sync::Thread;

int main() {
  Mutex m;
  Thread t([&]() {
    m.lock();
    m.unlock();
  });
  m.lock();
  m.unlock();
  return 0;
}
