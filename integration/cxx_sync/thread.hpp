#ifndef SYNC_ANALYSIS_INTEGRATION_CXX_SYNC_THREAD_HPP_
#define SYNC_ANALYSIS_INTEGRATION_CXX_SYNC_THREAD_HPP_

#include <pthread.h>

namespace sync {

class Thread {
public:
  template <class Func> explicit Thread(Func callable) {
    init_thread(&proxy<Func>, new Func(static_cast<Func&&>(callable)));
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

  pthread_t pt_thread = nullptr;

  template <class Func> static void* proxy(void* arg) {
    auto func = static_cast<Func*>(arg);
    (*func)();
    delete func;
    return nullptr;
  }
};

} // namespace sync

#endif
