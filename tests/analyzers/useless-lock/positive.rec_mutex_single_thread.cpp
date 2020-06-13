#include <cxxsync/recursive_mutex.hpp>

using sync::RecursiveMutex;

int main() {
  RecursiveMutex m;
  for (int i = 0; i < 100; i++) {
    m.lock();
  }
  m.unlock();
  return 0;
}
