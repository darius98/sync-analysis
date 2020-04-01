#include "thread.hpp"

#include <sync_analysis.h>

#include "exception.hpp"

namespace sync {

void Thread::sleep_ns(long long nanoseconds) {
  timespec sleep_duration{};
  sleep_duration.tv_sec = nanoseconds / 1000000000;
  sleep_duration.tv_nsec = nanoseconds % 1000000000;
  nanosleep(&sleep_duration, NULL);
}

void Thread::yield() {
  pthread_yield_np();
}

Thread::~Thread() {
  if (!is_null()) {
    join();
  }
}

Thread::Thread(Thread&& other) noexcept: pt_thread(other.pt_thread) {
  other.pt_thread = nullptr;
}

Thread& Thread::operator=(Thread&& other) noexcept {
  if (this != &other) {
    if (!is_null()) {
      join();
    }
    pt_thread = other.pt_thread;
    other.pt_thread = nullptr;
  }
  return *this;
}

bool Thread::is_null() const noexcept {
  return pt_thread == nullptr;
}

void Thread::join() {
  if (is_null()) {
    SyncException::throw_on_error("Thread", "join", 666013);
  }

  syan_thread_on_join(pt_thread);
  void* result;
  int status = pthread_join(pt_thread, &result);
  SyncException::throw_on_error("Thread", "pthread_join", status);
  pt_thread = nullptr;
}

void Thread::detach() {
  if (is_null()) {
    SyncException::throw_on_error("Thread", "detach", 666013);
  }

  syan_thread_on_detach(pt_thread);
  int status = pthread_detach(pt_thread);
  SyncException::throw_on_error("Thread", "pthread_detach", status);
  pt_thread = nullptr;
}

void Thread::init_thread(void* (*func)(void*), void* arg) {
  int status = pthread_create(&pt_thread, nullptr, func, arg);
  // TODO: This might happen AFTER other events on pt_thread.
  syan_thread_on_create(pt_thread);
  SyncException::throw_on_error("Thread", "pthread_create", status);
}

}  // namespace sync
