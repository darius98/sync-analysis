#include <executable/check.hpp>
#include <executable/utils.hpp>

using namespace syan;

class ListCheck : public Check {
public:
  void on_event(const syan::Environment& env, Event event) final {
    env.create_report(syan::Report::info, 1,
                      std::string{get_event_type_str(event)} + " -> " +
                          env.db().object_name(event) + ", from " +
                          env.db().thread_name(event))
        .send();
  }
};
SYAN_REGISTER_CHECK(ListCheck);
