#include <sync_analysis.h>

namespace ns {

struct ClassInNs {
  void method() {
    syan_capture_event(0, nullptr);
  }
};

void function_in_ns() {
  syan_capture_event(0, nullptr);
  ClassInNs().method();
}

}  // namespace ns

void function() {
  syan_capture_event(0, nullptr);
  ns::function_in_ns();
  ns::ClassInNs().method();
}

int main() {
  syan_capture_event(0, nullptr);
  function();
  ns::function_in_ns();
  ns::ClassInNs().method();
  return 0;
}
