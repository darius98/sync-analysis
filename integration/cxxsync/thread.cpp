#include "cxxsync/thread.hpp"

#include "../shared/events.hpp"
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

  void* result;
  int status = pthread_join(pt_thread, &result);
  SyncException::throw_on_error("Thread", "pthread_join", status);
  syan_thread_on_join((void*)pt_thread);
  pt_thread = nullptr;
}

void Thread::detach() {
  if (is_null()) {
    SyncException::throw_on_error("Thread", "detach", 666013);
  }

  int status = pthread_detach(pt_thread);
  SyncException::throw_on_error("Thread", "pthread_detach", status);
  syan_thread_on_detach((void*)pt_thread);
  pt_thread = nullptr;
}

void Thread::init_thread(void* (*func)(void*), void* arg) {
  void* event = syan_thread_on_create_init();
  int status = pthread_create(&pt_thread, nullptr, func, arg);
  syan_thread_on_create_finalize(event, (void*)pt_thread);
  SyncException::throw_on_error("Thread", "pthread_create", status);
}

}  // namespace sync
