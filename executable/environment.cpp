#include "environment.hpp"

#include "check.hpp"

namespace syan {

Environment::Environment(std::string binary_file_path,
                         const std::string& dump_file_path)
    : binary_file_path(std::move(binary_file_path)),
      dump_file_path(dump_file_path),
      dump_file_reader(dump_file_path) {}

void Environment::analyze() {
  while (!dump_file_reader.done()) {
    Event event = dump_file_reader.read();
    if (event.signature != SYAN_EVENT_SIGNATURE) {
      report_corrupt_file(event.signature);
    }
    for (auto* registered_check = internal::RegisteredCheck::get_head();
         registered_check != nullptr;
         registered_check = registered_check->next_check) {
      registered_check->check->on_event(*this, event);
    }
  }
}

void Environment::report_corrupt_file(std::int_fast32_t signature) {
  logger.log_fatal("Dump file " + dump_file_path +
                   " is corrupt. Found signature " + std::to_string(signature) +
                   " instead of " + std::to_string(SYAN_EVENT_SIGNATURE) + ".");
}

} // namespace syan
