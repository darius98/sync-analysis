#include <executable/check.hpp>

using namespace syan;

class ListCheck : public Check {
public:
  void on_event(const syan::Environment& env, Event event) final {
    env.create_report(syan::Report::info, 1,
                      std::string{event.type_str()} + " -> " +
                          env.db().object_name(event) + ", from " +
                          env.db().thread_name(event))
        .send();
  }
};
SYAN_REGISTER_CHECK(ListCheck);
