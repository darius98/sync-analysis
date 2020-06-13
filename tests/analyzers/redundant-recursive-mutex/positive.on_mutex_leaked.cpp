#include <vector>

#include <cxxsync/recursive_mutex.hpp>
#include <cxxsync/thread.hpp>

using sync::RecursiveMutex;
using sync::RecursiveMutexGuard;
using sync::Thread;

int main(int argc, char**) {
  RecursiveMutex* m = new RecursiveMutex();
  std::vector<int> protected_data;

  std::vector<Thread> threads;
  for (int i = 0; i < 10; ++i) {
    threads.emplace_back([i, argc, m, &protected_data]() {
      for (int j = 0; j < 100; ++j) {
        {
          RecursiveMutexGuard guard(m);
          protected_data.push_back(argc * (i + j) - j);
        }
        Thread::sleep_ns(100);
      }
    });
  }
  for (auto& thread : threads) {
    thread.join();
  }
  return 0;
}
