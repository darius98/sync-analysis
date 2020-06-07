#ifndef SYNC_ANALYSIS_INTEGRATION_CXX_SYNC_EXCEPTION_HPP_
#define SYNC_ANALYSIS_INTEGRATION_CXX_SYNC_EXCEPTION_HPP_

#include <stdexcept>

namespace sync {

class SyncException final: public std::exception {
public:
  static void throw_on_error(const char* type, const char* function,
                             int error_code);

  ~SyncException() noexcept final;

  const char* get_type() const noexcept;

  const char* get_function() const noexcept;

  int get_error_code() const noexcept;

  const char* what() const noexcept final;

private:
  SyncException(const char* type, const char* function, int error_code);

  const char* type;
  const char* function;
  int error_code;
  char* message;
};

}  // namespace sync

#endif
