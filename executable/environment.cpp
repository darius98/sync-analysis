#include "environment.hpp"

#include "check.hpp"

namespace syan {

Environment::Environment(std::string binary_file_path,
                         std::string dump_file_path)
    : binary_file_path(std::move(binary_file_path)),
      dump_file_path(std::move(dump_file_path)) {
  for (auto* registered_check = internal::RegisteredCheck::get_head();
       registered_check != nullptr;
       registered_check = registered_check->next_check) {
    enabled_checks.push_back(registered_check->check);
  }
}

void Environment::analyze() {
  EventFileReader dump_file_reader(dump_file_path);

  for (auto* check: enabled_checks){
    check->on_start(*this);
  }

  while (!dump_file_reader.done()) {
    EventPtr event = EventPtr::make(dump_file_reader.read());
    if (event->signature != SYAN_EVENT_SIGNATURE) {
      // It's ok for the last event to be corrupt, maybe something was broken.
      if (!dump_file_reader.done()) {
        logger.log_fatal("Dump file " + dump_file_path + " is corrupt.");
      } else {
        return;
      }
    }
    for (auto* check: enabled_checks){
      check->on_event(*this, event);
    }
    active_objects_db.insert(event);
  }

  for (auto* check: enabled_checks){
    check->on_end(*this);
  }
}

Report Environment::create_report(Report::Level level, int code,
                                  std::string description) const noexcept {
  return Report{this, level, code, std::move(description)};
}

const ActiveObjectsDb& Environment::db() const noexcept {
  return active_objects_db;
}

void Environment::send_report(Report::Level level, int code,
                              const std::string& report_message) const {
  switch (level) {
  case Report::info:
    logger.log_info(report_message);
    break;
  case Report::warning:
    logger.log_warn(report_message);
    break;
  case Report::error:
    logger.log_error(report_message);
    break;
  }
}

void Environment::symbolize_backtrace_to_stream(const Event& event,
                                                std::ostream& stream) const {
  // TODO: Implement this.
  for (const auto& pc : event.backtrace) {
    if (pc != 0) {
      stream << "\t\t" << pc << "\n";
    }
  }
}

} // namespace syan