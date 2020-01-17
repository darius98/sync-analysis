#include "report.hpp"

#include <sstream>

#include "environment.hpp"

namespace syan {

Report::Report(const Environment* env, Level level, int code,
               std::string description)
    : env(env), level(level), code(code), description(std::move(description)) {}

void Report::add_section(std::string section_description, const Event& event) {
  sections.emplace_back(std::move(section_description), event);
}

void Report::send() {
  std::stringstream builder;
  builder << description << " (E" << code << ")\n";
  for (auto& section : sections) {
    builder << section.description
            << "\n\tTimestamp: " << section.event.timestamp
            << "\n\tStacktrace:\n";
    env->symbolize_backtrace_to_stream(section.event, builder);
  }
  env->send_report(level, code, builder.str());
}

Report::ReportSection::ReportSection(std::string description,
                                     const Event& event)
    : description(std::move(description)), event(event) {}

} // namespace syan
