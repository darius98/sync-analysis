#include <check_api/check_api.hpp>

using namespace syan;

class ListCheck: public Check {
public:
  void on_event(Event event) final {
    create_report(syan::Report::info, 1,
                  std::string{event.type_str()} + " -> " +
                      database().object_name(event) + ", from " +
                      database().thread_name(event))
        .send();
  }
};
SYAN_REGISTER_CHECK(ListCheck);
