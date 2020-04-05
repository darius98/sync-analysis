#include <map>
#include <optional>

#include <syan_extension_api/syan_extension_api.hpp>

using namespace syan;

struct RWLockState {
  Event create_event;
  bool was_ever_rd_locked{false};
  bool was_ever_wr_locked{false};

  explicit RWLockState(Event create_event)
      : create_event(std::move(create_event)) {}

  ~RWLockState() {
    // Don't report if the lock is unused instead.
    if (!was_ever_rd_locked && !was_ever_wr_locked) {
      return;
    }
    if (!was_ever_wr_locked) {
      auto report = create_report();
      report.set_level(Report::Level::warning);
      report.set_description("Read-write lock was only used as a read lock. "
                             "Can be removed completely.");
      report.add_section("Read-write lock created here", create_event);
    }
    if (!was_ever_rd_locked) {
      auto report = create_report();
      report.set_level(Report::Level::warning);
      report.set_description("Read-write lock was only used as a write lock. "
                             "Can be replaced with a simple mutex.");
      report.add_section("Read-write lock created here", create_event);
    }
  }
};

struct RedundantRWLockExtension {
  static constexpr const char* name = "redundant-rwlock";

  std::map<ObjectId, RWLockState> rwlocks;

  void operator()(Event event) {
    if (event.object_type() != ObjectType::rwlock) {
      return;
    }
    if (event.is_create_event()) {
      rwlocks.emplace(event.object(), event);
      return;
    }

    auto it = rwlocks.find(event.object());
    if (it == rwlocks.end()) {
      return;
    }

    switch (event.type()) {
    case EventType::rwlock_after_rd_lock: {
      it->second.was_ever_rd_locked = true;
      break;
    }
    case EventType::rwlock_after_wr_lock: {
      it->second.was_ever_wr_locked = true;
      break;
    }
    case EventType::rwlock_on_destroy: {
      rwlocks.erase(it);
      break;
    }
    default: break;
    }
  }
};

SYAN_EXTENSION_SET_CLASS(RedundantRWLockExtension);
