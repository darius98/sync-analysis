#include <iostream>

#include "src/event_file_reader.hpp"

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cout << "Invalid arguments." << std::endl;
    return 1;
  }

  syan::EventFileReader reader(argv[1]);

  while (!reader.done()) {
    Event event = reader.read();
    std::cout << "Event of type " << event.event_type << ", at timestamp "
              << event.timestamp << ", on thread " << event.thread_id << "\n";
  }

  return 0;
}
