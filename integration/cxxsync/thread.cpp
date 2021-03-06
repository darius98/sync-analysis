#include "cxxsync/thread.hpp"

#include "../shared/events.h"
#include "exception.hpp"

namespace sync {

void Thread::sleep_ns(long long nanoseconds) {
  timespec sleep_duration{};
  sleep_duration.tv_sec = nanoseconds / 1000000000;
  sleep_duration.tv_nsec = nanoseconds % 1000000000;
  nanosleep(&sleep_duration, NULL);
}

Thread::~Thread() {
  if (!is_null()) {
    join();
  }
}

Thread::Thread(Thread&& other) noexcept: pt_thread(other.pt_thread) {
  other.pt_thread = (pthread_t)nullptr;
}

Thread& Thread::operator=(Thread&& other) noexcept {
  if (this != &other) {
    if (!is_null()) {
      join();
    }
    pt_thread = other.pt_thread;
    other.pt_thread = (pthread_t)nullptr;
  }
  return *this;
}

bool Thread::is_null() const noexcept {
  return !pt_thread;
}

void Thread::join() {
  if (is_null()) {
    SyncException::throw_on_error("Thread", "join", 666013);
  }

  void* result;
  int status = pthread_join(pt_thread, &result);
  SyncException::throw_on_error("Thread", "pthread_join", status);
  syan_capture_event(SA_EV_THREAD_ON_JOIN, (void*)pt_thread);
  pt_thread = (pthread_t)nullptr;
}

void Thread::detach() {
  if (is_null()) {
    SyncException::throw_on_error("Thread", "detach", 666013);
  }

  int status = pthread_detach(pt_thread);
  SyncException::throw_on_error("Thread", "pthread_detach", status);
  syan_capture_event(SA_EV_THREAD_ON_DETACH, (void*)pt_thread);
  pt_thread = (pthread_t)nullptr;
}

void Thread::init_thread(void* (*func)(void*), void* arg) {
  void* event = syan_initialize_event(SA_EV_THREAD_ON_CREATE);
  int status = pthread_create(&pt_thread, nullptr, func, arg);
  syan_finalize_event(event, (void*)pt_thread);
  SyncException::throw_on_error("Thread", "pthread_create", status);
}

}  // namespace sync
