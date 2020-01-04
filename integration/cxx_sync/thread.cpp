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

  void* result;
  int status = pthread_join(pt_thread, &result);
  SyncException::throw_on_error("Thread", "pthread_join", status);
  syan_thread_on_join(this);
  pt_thread = nullptr;
}

void Thread::detach() {
  if (is_null()) {
    SyncException::throw_on_error("Thread", "detach", 666013);
  }

  int status = pthread_detach(pt_thread);
  SyncException::throw_on_error("Thread", "pthread_detach", status);
  syan_thread_on_detach(this);
  pt_thread = nullptr;
}

void Thread::init_thread(void* (*func)(void*), void* arg) {
  pthread_attr_t attr{};
  int status = pthread_attr_init(&attr);
  SyncException::throw_on_error("Thread", "pthread_attr_init", status);
  status = pthread_create(&pt_thread, &attr, func, arg);
  SyncException::throw_on_error("Thread", "pthread_create", status);
  status = pthread_attr_destroy(&attr);
  SyncException::throw_on_error("Thread", "pthread_attr_destroy", status);
}

void Thread::on_thread_start() {
  syan_thread_on_create(this);
}

} // namespace sync
