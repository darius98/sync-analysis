#include "thread.hpp"

#include <sync_analysis.h>

#include "exception.hpp"

namespace sync {

Thread::~Thread() {
  if (!is_null()) {
    join();
  }
}

bool Thread::is_null() const noexcept {
  return pt_thread == nullptr;
}

void Thread::join() {
  if (is_null()) {
    SyncException::throw_on_error("Thread", "join", 666013);
  }

  syan_thread_on_join(this);
  void* result;
  int status = pthread_join(pt_thread, &result);
  SyncException::throw_on_error("Thread", "pthread_join", status);
  pt_thread = nullptr;
}

void Thread::detach() {
  if (is_null()) {
    SyncException::throw_on_error("Thread", "detach", 666013);
  }

  syan_thread_on_detach(this);
  int status = pthread_detach(pt_thread);
  SyncException::throw_on_error("Thread", "pthread_detach", status);
  pt_thread = nullptr;
}

void Thread::init_thread(void* (*func)(void*), void* arg) {
  int status = pthread_create(&pt_thread, nullptr, func, arg);
  SyncException::throw_on_error("Thread", "pthread_create", status);
}

void Thread::on_thread_start() {
  syan_thread_on_create(this);
}

} // namespace sync
