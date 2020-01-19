#include <executable/check.hpp>

using namespace syan;

class ListCheck : public Check {
public:
  void on_event(const syan::Environment& env, EventPtr event) final {
    auto report = env.create_report(
        syan::Report::info, 1,
        "There was an event of type " + std::to_string(event->event_type) +
            " on thread " + std::to_string(event->thread_id) + ", on object " +
            std::to_string(event->addr));
    report.send();
  }
};
SYAN_REGISTER_CHECK(ListCheck);
