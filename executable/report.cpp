#include "report.hpp"

#include <iomanip>
#include <sstream>

#include "environment.hpp"

namespace syan {

Report::Report(Environment* env, Level level, int code, std::string description)
    : env(env), level(level), code(code), description(std::move(description)) {}

void Report::add_section(std::string section_description, Event event) {
  thread_notes.insert(event.thread());
  if (!event.is_create_event()) {
    notes.emplace(env->db().object_create(event));
  }
  sections.emplace_back(std::move(section_description), std::move(event));
}

void Report::send() {
  for (const auto& note : notes) {
    sections.emplace_back(std::string{"Note: "} + env->db().object_name(note) +
                              " was created here: ",
                          note);
  }

  for (const auto& thread_note : thread_notes) {
    sections.emplace_back("Note: " + env->db().thread_name(thread_note) +
                              " was created here:",
                          env->db().thread_create(thread_note));
  }

  std::stringstream builder;
  builder << description << " (E" << code << ")";
  for (auto& section : sections) {
    auto timestamp = env->file_header.start_time;

    timestamp.tv_sec += section.event.time_rel_to_program_start() / 1000000000;
    timestamp.tv_nsec += section.event.time_rel_to_program_start() % 1000000000;

    timestamp.tv_sec += timestamp.tv_nsec / 1000000000;
    timestamp.tv_nsec %= 1000000000;
    tm* calendarTime = gmtime(&timestamp.tv_sec);
    builder << "\n\t[" << std::put_time(calendarTime, "%d/%m/%Y %H:%M:%S")
            << "." << std::setfill('0') << std::setw(6)
            << timestamp.tv_nsec / 1000 << std::setw(0) << std::setfill(' ')
            << "] " << section.description << "\n";
    env->symbolize_stacktrace(section.event, builder);
  }
  env->send_report(level, code, builder.str());
}

Report::ReportSection::ReportSection(std::string description, Event event)
    : description(std::move(description)), event(std::move(event)) {}

}  // namespace syan
