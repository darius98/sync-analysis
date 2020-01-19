#ifndef SYNC_ANALYSIS_EXE_UTILS_H_
#define SYNC_ANALYSIS_EXE_UTILS_H_

#include <string_view>

#include "event.hpp"

namespace syan {

constexpr std::string_view thread_object_type = "thread";
constexpr std::string_view mutex_object_type = "mutex";
constexpr std::string_view rec_mutex_object_type = "recursive mutex";
constexpr std::string_view rwlock_object_type = "read-write lock";

bool is_create_event(const Event& event) noexcept;

std::string_view get_object_type(const Event& event) noexcept;

std::string_view get_event_type_str(const Event& event) noexcept;

}

#endif
