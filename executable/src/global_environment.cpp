#include "global_environment.hpp"

#include "environment.hpp"
#include "syan_analyzer_api/syan_analyzer_api.hpp"

namespace syan {

namespace {

Environment* environment = nullptr;

}  // namespace

void set_global_environment(Environment* env) {
  environment = env;
}

Event current_event() {
  return environment->current_event();
}

const Database& database() {
  return environment->get_database();
}

Report create_report() {
  return Report{};
}

struct timespec execution_start_time() {
  return environment->execution_start_time();
}

std::string_view active_analyzer_name() {
  return environment->active_analyzer_name();
}

void symbolize_stacktrace(const Event& event, std::ostream& stream) {
  environment->symbolize_stacktrace(event, stream);
}

void send_report(Report::Level level, const std::string& report_message) {
  environment->send_report(level, report_message);
}

}  // namespace syan
