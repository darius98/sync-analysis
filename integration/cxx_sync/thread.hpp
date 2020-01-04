#ifndef SYNC_ANALYSIS_INTEGRATION_CXX_SYNC_THREAD_HPP_
#define SYNC_ANALYSIS_INTEGRATION_CXX_SYNC_THREAD_HPP_

#include <pthread.h>

namespace sync {

class Thread {
public:
  template <class Func> explicit Thread(Func callable) {
    auto context = new Context<Func>{this, static_cast<Func&&>(callable)};
    init_thread(&proxy<Func>, context);
  }

  ~Thread();

  bool is_null() const noexcept;

  void join();

  void detach();

private:
  template <class Func> struct Context {
    Thread* thread;
    Func callable;
  };

  void init_thread(void* (*func)(void*), void* arg);

  void on_thread_start();

  pthread_t pt_thread = nullptr;

  template <class Func> static void* proxy(void* arg) {
    auto context = static_cast<Context<Func>*>(arg);
    context->thread->on_thread_start();
    context->callable();
    delete context;
    return nullptr;
  }
};

} // namespace sync

#endif
