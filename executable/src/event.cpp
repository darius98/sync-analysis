#include "syan_extension_api/event.hpp"

#include <lib/src/event.h>

namespace syan {

struct Event::EventPtrInternal {
  int ref_count = 1;
  ::SyanEvent event;
};

EventType Event::type() const noexcept {
  return static_cast<EventType>(ptr->event.event_type);
}

ObjectId Event::object() const noexcept {
  return ptr->event.addr;
}

ObjectId Event::thread() const noexcept {
  return ptr->event.thread_id;
}

std::int64_t Event::time_rel_to_program_start() const noexcept {
  return ptr->event.timestamp;
}

RawBacktrace Event::raw_backtrace() const noexcept {
  return ptr->event.backtrace;
}

Event Event::make(const void* syan_event) {
  const ::SyanEvent& event = *static_cast<const ::SyanEvent*>(syan_event);
  if (event.signature != SYAN_EVENT_SIGNATURE) {
    return Event{};
  }
  Event event_ptr;
  event_ptr.ptr = new EventPtrInternal{1, event};
  return event_ptr;
}

void Event::inc_ref_count() {
  if (ptr != nullptr) {
    ptr->ref_count += 1;
  }
}

void Event::dec_ref_count() {
  if (ptr != nullptr) {
    ptr->ref_count -= 1;
    if (ptr->ref_count == 0) {
      delete ptr;
    }
  }
}

}  // namespace syan