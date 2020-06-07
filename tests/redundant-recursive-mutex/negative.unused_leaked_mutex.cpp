#include <cxxsync/recursive_mutex.hpp>

using sync::RecursiveMutex;

int main() {
  auto m = new RecursiveMutex();
  (void)m;
  return 0;
}
