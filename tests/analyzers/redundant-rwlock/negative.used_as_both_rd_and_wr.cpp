#include <iostream>
#include <numeric>
#include <vector>

#include <cxxsync/rwlock.hpp>
#include <cxxsync/thread.hpp>

using sync::RWLock;
using sync::RWLockReadGuard;
using sync::RWLockWriteGuard;
using sync::Thread;

int main(int argc, char**) {
  RWLock m;
  std::vector<int> protected_data;

  std::vector<Thread> threads;
  for (int i = 0; i < 10; ++i) {
    threads.emplace_back([i, argc, &m, &protected_data]() {
      for (int j = 0; j < 100; ++j) {
        {
          RWLockWriteGuard guard(&m);
          protected_data.push_back(argc * (i + j) - j);
        }
        Thread::sleep_ns(100);
      }
    });
  }
  for (int i = 0; i < 5; ++i) {
    threads.emplace_back([i, argc, &m, &protected_data]() {
      for (int j = 0; j < 5; ++j) {
        {
          RWLockReadGuard guard(&m);
          std::cout << "Side effect: "
                    << i + argc +
                           std::accumulate(protected_data.begin(),
                                           protected_data.end(), 0,
                                           std::plus<int>{})
                    << std::endl;
        }
        Thread::sleep_ns(2000);
      }
    });
  }
  for (auto& thread : threads) {
    thread.join();
  }
  return 0;
}
