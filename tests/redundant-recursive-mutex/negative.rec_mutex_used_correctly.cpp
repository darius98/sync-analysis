#include <vector>

#include <cxx_sync/recursive_mutex.hpp>

using sync::RecursiveMutex;
using sync::RecursiveMutexGuard;

void recursive_function(RecursiveMutex* m, std::vector<int>* protected_data,
                        int v) {
  RecursiveMutexGuard guard(m);
  protected_data->push_back(v);
  if (v != 0) {
    recursive_function(m, protected_data, v - 1);
  }
  protected_data->push_back(v);
}

int main(int argc, char**) {
  RecursiveMutex m;
  std::vector<int> protected_data;

  recursive_function(&m, &protected_data, argc + 5);
  return 0;
}
