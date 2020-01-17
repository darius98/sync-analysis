#include <iostream>

#include "check.hpp"
#include "event_file_reader.hpp"

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cout << "Invalid arguments." << std::endl;
    return 1;
  }

  syan::EventFileReader reader(argv[1]);

  while (!reader.done()) {
    Event event = reader.read();

    for (auto* registered_check = syan::internal::RegisteredCheck::get_head();
         registered_check != nullptr;
         registered_check = registered_check->next_check) {
      registered_check->check->on_event(event);
    }
  }

  return 0;
}
