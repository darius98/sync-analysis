#include <vector>

#include <cxx_sync/rwlock.hpp>

using sync::RWLock;
using sync::RWLockReadGuard;

int main(int argc, char**) {
  std::vector<int> pseudo_protected_data;
  RWLock lock;

  for (int i = 0; i < 100 + argc; ++i) {
    RWLockReadGuard guard(&lock);
    pseudo_protected_data.push_back(i + argc);
  }
  return 0;
}
