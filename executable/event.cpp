#include "event.hpp"

namespace syan {

EventPtr EventPtr::make(const Event& event) {
  EventPtr event_ptr;
  event_ptr.ptr = new EventPtrInternal{1, event};
  return event_ptr;
}

EventPtr::EventPtr() noexcept : ptr(nullptr) {}

EventPtr::EventPtr(decltype(nullptr)) noexcept : ptr(nullptr) {}

EventPtr::EventPtr(const EventPtr& other) noexcept : ptr(other.ptr) {
  if (ptr != nullptr) {
    ptr->ref_count += 1;
  }
}

EventPtr::EventPtr(EventPtr&& other) noexcept : ptr(other.ptr) {
  other.ptr = nullptr;
}

EventPtr& EventPtr::operator=(const EventPtr& other) noexcept {
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

EventPtr& EventPtr::operator=(EventPtr&& other) noexcept {
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

EventPtr::~EventPtr() noexcept {
  if (ptr != nullptr) {
    ptr->ref_count -= 1;
    if (ptr->ref_count == 0) {
      delete ptr;
    }
  }
}

Event& EventPtr::operator*() const noexcept {
  return ptr->event;
}

Event* EventPtr::operator->() const noexcept {
  return &ptr->event;
}

EventPtr::operator bool() const noexcept {
  return ptr != nullptr;
}

bool EventPtr::operator==(decltype(nullptr)) const noexcept {
  return ptr == nullptr;
}

bool EventPtr::operator!=(decltype(nullptr)) const noexcept {
  return ptr != nullptr;
}

bool EventPtr::operator<(const EventPtr& other) const noexcept {
  return ptr < other.ptr;
}

bool EventPtr::operator>(const EventPtr& other) const noexcept {
  return ptr > other.ptr;
}

bool EventPtr::operator<=(const EventPtr& other) const noexcept {
  return ptr <= other.ptr;
}

bool EventPtr::operator>=(const EventPtr& other) const noexcept {
  return ptr >= other.ptr;
}

bool EventPtr::operator==(const EventPtr& other) const noexcept {
  return ptr == other.ptr;
}

bool EventPtr::operator!=(const EventPtr& other) const noexcept {
  return ptr != other.ptr;
}

} // namespace syan