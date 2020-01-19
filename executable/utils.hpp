#ifndef SYNC_ANALYSIS_EXE_UTILS_H_
#define SYNC_ANALYSIS_EXE_UTILS_H_

#include "event.hpp"

namespace syan {

constexpr const char* thread_object_type = "thread";
constexpr const char* mutex_object_type = "mutex";
constexpr const char* rec_mutex_object_type = "recursive mutex";
constexpr const char* rwlock_object_type = "read-write lock";

bool is_create_event(const Event& event) noexcept;

const char* get_object_type(const Event& event) noexcept;

const char* get_event_type_str(const Event& event) noexcept;

}

#endif
