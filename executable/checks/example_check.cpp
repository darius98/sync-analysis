#include <iostream>

#include <executable/check.hpp>

class ExampleCheck: public syan::Check {
public:
  void on_event(const syan::Environment& env, const Event &event) final {
    std::cout << "Event of type " << event.event_type << ", at timestamp "
              << event.timestamp << ", on thread " << event.thread_id << "\n";
  }
};
SYAN_REGISTER_CHECK(ExampleCheck);
