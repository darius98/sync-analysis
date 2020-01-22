#include "event.hpp"

namespace syan {

Event Event::make(const ::SyanEvent& event) {
  if (event.signature != SYAN_EVENT_SIGNATURE) {
    return Event{};
  }
  Event event_ptr;
  event_ptr.ptr = new EventPtrInternal{1, event};
  return event_ptr;
}

Event::Event() noexcept : ptr(nullptr) {}

Event::Event(decltype(nullptr)) noexcept : ptr(nullptr) {}

Event::Event(const Event& other) noexcept : ptr(other.ptr) {
  if (ptr != nullptr) {
    ptr->ref_count += 1;
  }
}

Event::Event(Event&& other) noexcept : ptr(other.ptr) {
  other.ptr = nullptr;
}

Event& Event::operator=(const Event& other) noexcept {
  if (this == &other) {
    return *this;
  }

  if (ptr != nullptr) {
    ptr->ref_count -= 1;
    if (ptr->ref_count == 0) {
      delete ptr;
    }
  }
  ptr = other.ptr;
  if (ptr != nullptr) {
    ptr->ref_count += 1;
  }
  return *this;
}

Event& Event::operator=(Event&& other) noexcept {
  if (this == &other) {
    return *this;
  }

  if (ptr != nullptr) {
    ptr->ref_count -= 1;
    if (ptr->ref_count == 0) {
      delete ptr;
    }
  }
  ptr = other.ptr;
  other.ptr = nullptr;
  return *this;
}

Event::~Event() noexcept {
  if (ptr != nullptr) {
    ptr->ref_count -= 1;
    if (ptr->ref_count == 0) {
      delete ptr;
    }
  }
}

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

Event::operator bool() const noexcept {
  return ptr != nullptr;
}

bool Event::operator==(decltype(nullptr)) const noexcept {
  return ptr == nullptr;
}

bool Event::operator!=(decltype(nullptr)) const noexcept {
  return ptr != nullptr;
}

bool Event::operator<(const Event& other) const noexcept {
  return ptr < other.ptr;
}

bool Event::operator>(const Event& other) const noexcept {
  return ptr > other.ptr;
}

bool Event::operator<=(const Event& other) const noexcept {
  return ptr <= other.ptr;
}

bool Event::operator>=(const Event& other) const noexcept {
  return ptr >= other.ptr;
}

bool Event::operator==(const Event& other) const noexcept {
  return ptr == other.ptr;
}

bool Event::operator!=(const Event& other) const noexcept {
  return ptr != other.ptr;
}

} // namespace syan