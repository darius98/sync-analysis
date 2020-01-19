#include "report.hpp"

#include <sstream>

#include "environment.hpp"

namespace syan {

Report::Report(const Environment* env, Level level, int code,
               std::string description) noexcept
    : env(env), level(level), code(code), description(std::move(description)) {}

void Report::add_section(std::string section_description, EventPtr event) {
  thread_notes.insert(event->thread_id);
  sections.emplace_back(std::move(section_description), std::move(event));
}

void Report::add_mutex_note(ObjectId mutex_id) {
  add_unique_object_note("Mutex", env->db().mutex_create(mutex_id));
}

void Report::send() {
  for (const auto& object_note : object_notes) {
    sections.emplace_back(std::string{"Note: "} + std::get<0>(object_note) +
                              " " +
                              std::to_string(std::get<1>(object_note)->addr) +
                              " was created here: ",
                          std::get<1>(object_note));
  }

  // TODO: Thread create stacktrace is not that useful, we need the stacktrace
  //  on the creating thread. Split the event SA_EV_THREAD_ON_CREATE in two:
  //  SA_EV_THREAD_BEFORE_CREATE and SA_EV_THREAD_AFTER_CREATE (one on the
  //  spawning thread and one on the spawned thread) and print the stacktrace on
  //  the spawning thread instead.
  for (const auto& thread_note : thread_notes) {
    sections.emplace_back("Note: Thread " + env->db().thread_name(thread_note) +
                              " was created here:",
                          env->db().thread_create(thread_note));
    auto thread_detach_event = env->db().thread_detach(thread_note);
    if (thread_detach_event != nullptr) {
      sections.emplace_back("Note: Thread " +
                                env->db().thread_name(thread_note) +
                                " was detached here:",
                            thread_detach_event);
    }
  }

  std::stringstream builder;
  builder << description << " (E" << code << ")\n";
  for (auto& section : sections) {
    builder << "\t[" << section.event->timestamp << "] " << section.description
            << "\n";
    env->symbolize_backtrace_to_stream(*section.event, builder);
  }
  env->send_report(level, code, builder.str());
}

void Report::add_unique_object_note(const char* object_type, EventPtr event) {
  object_notes.emplace(object_type, std::move(event));
}

Report::ReportSection::ReportSection(std::string description, EventPtr event)
    : description(std::move(description)), event(std::move(event)) {}

} // namespace syan
