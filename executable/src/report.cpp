#include "syan_analyzer_api/report.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>

#include "run_analysis.hpp"
#include "syan_analyzer_api/syan_analyzer_api.hpp"

namespace syan {

Report::~Report() {
  send();
}

Report& Report::set_level(Level new_level) noexcept {
  level = new_level;
  return *this;
}

Report& Report::set_description(std::string new_description) noexcept {
  description = std::move(new_description);
  return *this;
}

Report& Report::add_section(std::string section_description, Event event) {
  thread_notes.insert(event.thread());
  if (!event.is_create_event()) {
    notes.emplace(database().object_create(event));
  }
  sections.emplace_back(std::move(section_description), std::move(event));
  return *this;
}

void Report::send() {
  if (is_sent) {
    return;
  }
  is_sent = true;

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
  switch (level) {
  case Report::info: builder << "INFO"; break;
  case Report::warning: builder << "WARNING"; break;
  case Report::error: std::cout << "ERROR"; break;
  }
  builder << " (" << active_analyzer_name() << "): " << description;
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
            << "] " << section.description;
    symbolize_stacktrace(section.event, builder);
  }

  send_report(level, builder.str());
}

}  // namespace syan
