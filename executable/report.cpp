#include <check_api/report.hpp>

#include <iomanip>
#include <sstream>

#include "run_analysis.hpp"
#include <check_api/check_api.hpp>

namespace syan {

Report::Report(Level level, int code, std::string description)
    : level(level), code(code), description(std::move(description)) {}

Report& Report::add_section(std::string section_description, Event event) {
  thread_notes.insert(event.thread());
  if (!event.is_create_event()) {
    notes.emplace(database().object_create(event));
  }
  sections.emplace_back(std::move(section_description), std::move(event));
  return *this;
}

Report::~Report() {
  for (const auto& note : notes) {
    sections.emplace_back(
        "Note: " + database().object_name(note) + " was created here: ", note);
  }

  for (const auto& note : thread_notes) {
    sections.emplace_back("Note: " + database().thread_name(note) +
                              " was created here: ",
                          database().thread_create(note));
  }

  std::stringstream builder;
  builder << description << " (E" << code << ")";
  for (auto& section : sections) {
    auto timestamp = execution_start_time();

    timestamp.tv_sec += section.event.time_rel_to_program_start() / 1000000000;
    timestamp.tv_nsec += section.event.time_rel_to_program_start() % 1000000000;

    timestamp.tv_sec += timestamp.tv_nsec / 1000000000;
    timestamp.tv_nsec %= 1000000000;
    tm* calendarTime = gmtime(&timestamp.tv_sec);
    builder << "\n\t[" << std::put_time(calendarTime, "%d/%m/%Y %H:%M:%S")
            << "." << std::setfill('0') << std::setw(6)
            << timestamp.tv_nsec / 1000 << std::setw(0) << std::setfill(' ')
            << "] " << section.description << "\n";
    symbolize_stacktrace(section.event, builder);
  }

  send_report(level, code, builder.str());
}

}  // namespace syan
