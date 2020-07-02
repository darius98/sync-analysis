#include <benchmark/benchmark.h>
#include <atomic>
#include <numeric>
#include <thread>
#include <vector>

#include <sync_analysis.h>

static void throughput(benchmark::State& state) {
  const auto num_background_threads = state.range(0) - 1;
  std::vector<std::thread> background_threads;
  std::atomic_bool stop_token{false};
  std::vector<int> background_counters(num_background_threads, 0);
  for (int i = 0; i < num_background_threads; i++) {
    background_threads.emplace_back([&stop_token, &background_counters, i]() {
      while (!stop_token.load(std::memory_order_acquire)) {
        syan_capture_event(199, nullptr);
        background_counters[i]++;
      }
    });
  }

  int counter = 0;
  while (state.KeepRunning()) {
    syan_capture_event(199, nullptr);
    counter++;
  }

  stop_token.store(true, std::memory_order_release);
  for (auto& background_thread : background_threads) {
    background_thread.join();
  }

  state.SetItemsProcessed(std::accumulate(background_counters.begin(),
                                          background_counters.end(), 0) +
                          counter);
}
BENCHMARK(throughput)
    ->ArgName("num_threads")
    ->Arg(1)
    ->Arg(2)
    ->Arg(3)
    ->Arg(4)
    ->Arg(5)
    ->Arg(6)
    ->Arg(7);
