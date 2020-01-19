#ifndef SYNC_ANALYSIS_EXE_EVENT_H_
#define SYNC_ANALYSIS_EXE_EVENT_H_

#include <lib/src/event.h>
#include <lib/src/event_type.h>

namespace syan {

using EventType = ::EventType;
using Event = ::Event;
using ObjectId = decltype(Event::addr);
using ThreadId = decltype(Event::thread_id);

// A shared_ptr implementation that is:
// - without thread-safety guarantees
// - without support for weak_ptr
// - without support for taking ownership of an existing pointer
class EventPtr {
public:
  static EventPtr make(const Event& event);

  explicit EventPtr() noexcept;
  EventPtr(decltype(nullptr)) noexcept;

  EventPtr(const EventPtr& other) noexcept;
  EventPtr(EventPtr&& other) noexcept;

  EventPtr& operator=(const EventPtr& other) noexcept;
  EventPtr& operator=(EventPtr&& other) noexcept;

  ~EventPtr() noexcept;

  Event& operator*() const noexcept;

  Event* operator->() const noexcept;

  explicit operator bool() const noexcept;

  bool operator==(decltype(nullptr)) const noexcept;
  bool operator!=(decltype(nullptr)) const noexcept;

  bool operator<(const EventPtr& other) const noexcept;
  bool operator>(const EventPtr& other) const noexcept;
  bool operator<=(const EventPtr& other) const noexcept;
  bool operator>=(const EventPtr& other) const noexcept;
  bool operator==(const EventPtr& other) const noexcept;
  bool operator!=(const EventPtr& other) const noexcept;

private:
  struct EventPtrInternal {
    int ref_count = 1;
    Event event;
  };

  EventPtrInternal* ptr;
};

} // namespace syan

#endif
