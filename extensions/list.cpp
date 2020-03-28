#include <syan_extension_api/syan_extension_api.hpp>

using namespace syan;

SYAN_EXT_API const char* syan_extension = "list";

SYAN_EXT_API void syan_extension_on_event() {
  auto event = current_event();
  Report(syan::Report::info, 1,
         std::string{event.type_str()} + " -> " +
             database().object_name(event) + ", from " +
             database().thread_name(event));
}
