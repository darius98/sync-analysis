#include <map>
#include <optional>

#include <syan_analyzer_api/syan_analyzer_api.hpp>

using namespace syan;

struct ObjectState {
  Event create_event;
  std::optional<ObjectId> events_thread{std::nullopt};
  bool multiple_threads = false;

  explicit ObjectState(Event create_event): create_event(create_event) {}

  ~ObjectState() {
    if (!events_thread.has_value()) {
      auto report = create_report();
      report.set_level(Report::Level::warning);
      report.set_description(database().object_name(create_event) +
                             " is never locked.");
      report.add_section(database().object_name(create_event) +
                             " was created here:",
                         create_event);
      return;
    }
    if (!multiple_threads) {
      auto report = create_report();
      report.set_level(Report::Level::warning);
      report.set_description(database().object_name(create_event) +
                             " is only locked on one thread.");
      report.add_section(database().object_name(create_event) +
                             " was created here:",
                         create_event);
      report.add_section("All events happened on thread " +
                             database().thread_name(events_thread.value()),
                         database().thread_create(events_thread.value()));
    }
  }

  bool set_thread(ObjectId new_events_thread) {
    if (!events_thread.has_value()) {
      events_thread.emplace(new_events_thread);
      return true;
    }
    if (events_thread.value() == new_events_thread) {
      return true;
    }
    multiple_threads = true;
    return false;
  }
};

struct SingleThreadLock {
  static constexpr const char* name = "useless-lock";

  std::map<std::pair<ObjectType, ObjectId>, ObjectState> objects;

  void operator()(Event event) {
    if (event.raw_type() & event_type_thread) {
      return;
    }

    const auto object_key = std::pair{event.object_type(), event.object()};

    if (event.is_create_event()) {
      objects.emplace(std::piecewise_construct,
                      std::forward_as_tuple(object_key),
                      std::forward_as_tuple(event));
      return;
    }
    if (event.is_destroy_event()) {
      objects.erase(object_key);
      return;
    }

    if ((event.raw_type() & event_type_before_rd_lock) ||
        (event.raw_type() & event_type_before_wr_lock) ||
        (event.raw_type() & event_type_try_rd_lock) ||
        (event.raw_type() & event_type_try_wr_lock)) {
      auto it = objects.find(object_key);
      if (it != objects.end() && !it->second.set_thread(event.thread())) {
        objects.erase(it);
      }
    }
  }
};

SYAN_ANALYZER_CLASS(SingleThreadLock);
