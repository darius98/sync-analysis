#include <benchmark/benchmark.h>
#include <thread>
#include <vector>
#include <atomic>

#include <sync_analysis.h>

static void latency(benchmark::State& state) {
  const auto num_background_threads = state.range(0);
  std::vector<std::thread> background_threads;
  std::atomic_bool stop_token{false};
  for (int i = 0; i < num_background_threads; i++) {
    background_threads.emplace_back([&stop_token]() {
      while (!stop_token.load(std::memory_order_acquire)) {
        syan_capture_event(199, nullptr);
        std::this_thread::yield();
      }
    });
  }

  while (state.KeepRunning()) {
    syan_capture_event(200, nullptr);
  }

  stop_token.store(true, std::memory_order_release);
  for (auto& background_thread : background_threads) {
    background_thread.join();
  }
}
BENCHMARK(latency)
    ->ArgName("background_noise_threads")
    ->Arg(0)
    ->Arg(1)
    ->Arg(2)
    ->Arg(3)
    ->Arg(4)
    ->Arg(5)
    ->Arg(6);
