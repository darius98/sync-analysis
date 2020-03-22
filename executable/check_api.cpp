#include "include/check_api/check_api.hpp"

#include "environment.hpp"

namespace {

syan::Environment* env = nullptr;

}

namespace syan {

const Database& database() noexcept {
  return env->db();
}

Report create_report(Report::Level level, int code, std::string description) {
  return env->create_report(level, code, std::move(description));
}

void run_analysis(std::optional<std::string> binary_file_path,
                  std::string dump_file_path) {
  env = new Environment{std::move(binary_file_path), std::move(dump_file_path)};
  env->analyze();
}

}  // namespace syan
