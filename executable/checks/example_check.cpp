#include <executable/check.hpp>

class ExampleCheck : public syan::Check {
public:
  void on_event(const syan::Environment& env, const Event& event) final {
    auto report = env.create_report(syan::Report::info, 1,
                                    "There was an event of type " +
                                        std::to_string(event.event_type));
    report.add_section("Event", event);
    report.send();
  }
};
SYAN_REGISTER_CHECK(ExampleCheck);
