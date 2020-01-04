#ifndef SYNC_ANALYSIS_INTEGRATION_CXX_SYNC_THREAD_HPP_
#define SYNC_ANALYSIS_INTEGRATION_CXX_SYNC_THREAD_HPP_

#include <pthread.h>

namespace sync {

class Thread {
public:
  template <class Callable> explicit Thread(Callable callable) {
    auto context = new Context{this, static_cast<Callable&&>(callable)};
    init_thread(&proxy<Callable>, context);
  }

  ~Thread();

  bool is_null() const noexcept;

  void join();

  void detach();

private:
  template <class Callable> struct Context {
    Thread* thread;
    Callable callable;
  };

  void init_thread(void* (*func)(void*), void* arg);

  void on_thread_start();

  pthread_t pt_thread = nullptr;

  template <class Callable> static void* proxy(void* arg) {
    auto context = static_cast<Context<Callable>*>(arg);
    context->thread->on_thread_start();
    context->callable();
    delete context;
    return nullptr;
  }
};

} // namespace sync

#endif
