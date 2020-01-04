#include "exception.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace sync {

void SyncException::throw_on_error(const char* type, const char* function,
                                   int error_code) {
  if (error_code != 0) {
    throw SyncException(type, function, error_code);
  }
}

SyncException::SyncException(const char* type, const char* function,
                             int error_code)
    : type(type), function(function), error_code(error_code),
      message(static_cast<char*>(std::malloc(
          std::strlen(type) + 2 + std::strlen(function) + 19 + 11))) {
  if (message == nullptr) {
    std::abort();
  }
  std::sprintf(message, "%s: %s failed with error %d", type, function,
               error_code);
}

SyncException::~SyncException() noexcept {
  std::free(message);
}

const char* SyncException::get_type() const noexcept {
  return type;
}

const char* SyncException::get_function() const noexcept {
  return function;
}

int SyncException::get_error_code() const noexcept {
  return error_code;
}

const char* SyncException::what() const noexcept {
  return message;
}

} // namespace sync
