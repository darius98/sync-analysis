#include "report.hpp"

#include <sstream>

#include "environment.hpp"
#include "utils.hpp"

namespace syan {

Report::Report(const Environment* env, Level level, int code,
               std::string description)
    : env(env), level(level), code(code), description(std::move(description)) {}

void Report::add_section(std::string section_description, EventPtr event) {
  thread_notes.insert(event->thread_id);
  if (!is_create_event(event)) {
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
    auto thread_detach_event = env->db().thread_detach(thread_note);
    if (thread_detach_event != nullptr) {
      sections.emplace_back("Note: " + env->db().thread_name(thread_note) +
                                " was detached here:",
                            thread_detach_event);
    }
  }

  std::stringstream builder;
  builder << description << " (E" << code << ")";
  for (auto& section : sections) {
    builder << "\n\t[" << section.event->timestamp << "] "
            << section.description << "\n";
    env->symbolize_backtrace_to_stream(*section.event, builder);
  }
  env->send_report(level, code, builder.str());
}

Report::ReportSection::ReportSection(std::string description, EventPtr event)
    : description(std::move(description)), event(std::move(event)) {}

} // namespace syan
