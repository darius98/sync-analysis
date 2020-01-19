#include "report.hpp"

#include <sstream>

#include "environment.hpp"

namespace syan {

Report::Report(const Environment* env, Level level, int code,
               std::string description) noexcept
    : env(env), level(level), code(code), description(std::move(description)) {}

void Report::add_section(std::string section_description, const Event& event) {
  sections.emplace_back(std::move(section_description), event);
  thread_notes.insert(event.thread_id);
}

void Report::add_mutex_note(const std::string& mutex_name, ObjectId mutex_id) {
  add_unique_object_note("Mutex", mutex_name,
                         env->db().get_mutex_create_event(mutex_id).value());
}

void Report::send() {
  for (const auto& object_note : object_notes) {
    sections.emplace_back(std::string{"Note: "} + std::get<0>(object_note) +
                              " " + std::get<1>(object_note) +
                              " was created here: ",
                          std::get<2>(object_note));
  }

  // TODO: Thread create stacktrace is not that useful, we need the stacktrace
  //  on the creating thread. Split the event SA_EV_THREAD_ON_CREATE in two:
  //  SA_EV_THREAD_BEFORE_CREATE and SA_EV_THREAD_AFTER_CREATE (one on the
  //  spawning thread and one on the spawned thread) and print the stacktrace on
  //  the spawning thread instead.
  for (const auto& thread_note : thread_notes) {
    sections.emplace_back(
        "Note: Thread " + env->db().get_thread_name(thread_note) +
            " was created here:",
        env->db().get_thread_create_event(thread_note).value());
    auto thread_detach_event = env->db().get_thread_detach_event(thread_note);
    if (thread_detach_event.has_value()) {
      sections.emplace_back("Note: Thread " +
                                env->db().get_thread_name(thread_note) +
                                " was detached here:",
                            thread_detach_event.value());
    }
  }

  std::stringstream builder;
  builder << description << " (E" << code << ")\n";
  for (auto& section : sections) {
    builder << "\t[" << section.event.timestamp << "] " << section.description
            << "\n";
    env->symbolize_backtrace_to_stream(section.event, builder);
  }
  env->send_report(level, code, builder.str());
}

void Report::add_unique_object_note(const char* object_type,
                                    const std::string& object_name,
                                    const Event& event) {
  object_notes.emplace(object_type, object_name, event);
}

Report::ReportSection::ReportSection(std::string description,
                                     const Event& event)
    : description(std::move(description)), event(event) {}

bool Report::ObjectNoteCompare::operator()(
    const std::tuple<const char*, std::string, Event>& t1,
    const std::tuple<const char*, std::string, Event>& t2) const noexcept {
  return (std::get<0>(t1) < std::get<0>(t2)) ||
         (std::get<0>(t1) == std::get<0>(t2) &&
          std::get<1>(t1) < std::get<1>(t2)) ||
         (std::get<0>(t1) == std::get<0>(t2) &&
          std::get<1>(t1) == std::get<1>(t2) &&
          std::get<2>(t1).addr == std::get<2>(t2).addr);
}

} // namespace syan
