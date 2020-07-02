#include "event_file_reader.hpp"

#include <iostream>
#include <stdexcept>

namespace syan {

EventFileReader::EventFileReader(std::string fn, std::size_t buffer_cap)
    : file_name(std::move(fn)),
      file(std::fopen(file_name.c_str(), "rb")),
      buffer_cap(buffer_cap),
      buffer_cursor(buffer_cap),
      is_done_reading_file(false) {
  auto buf = std::make_unique<::SyanEvent[]>(buffer_cap);
  if (file == nullptr) {
    std::cerr << "Cannot open dump file at '" << file_name << "'\n";
    std::abort();
  }
  buffer = std::move(buf);
  int read_status = read_header();
  if (read_status != 0) {
    std::cerr << "Cannot read dump file at '" << file_name << "'\n";
    std::abort();
  }
  read_next_chunk();
}

::SyanDumpFileHeader&& EventFileReader::release_header() noexcept {
  return std::move(file_header);
}

bool EventFileReader::done() const noexcept {
  return is_done_reading_file && buffer_cursor == buffer_cap;
}

Event EventFileReader::read() {
  if (done()) {
    return Event{};
  }
  if (buffer_cursor == buffer_cap) {
    read_next_chunk();
  }
  if (done()) {
    return Event{};
  }
  auto event = Event::make(&buffer[buffer_cursor]);
  std::cerr << "Event:";
  for (char* ptr = (char*)&buffer[buffer_cursor];
       ptr < ((char*)&buffer[buffer_cursor] + sizeof(SyanEvent)); ptr += 4) {
    std::cerr << " " << *(int*)ptr;
  }
  std::cerr << "\n";
  buffer_cursor++;
  return event;
}

int EventFileReader::read_header() {
  size_t total_bytes = 0;

  size_t num_read;
  num_read =
      fread(&file_header.start_time, sizeof(struct timespec), 1, file.get());
  total_bytes += sizeof(struct timespec) * 1;
  if (num_read != 1) {
    return 1;
  }

  num_read =
      fread(&file_header.program_name_length, sizeof(size_t), 1, file.get());
  total_bytes += sizeof(size_t) * 1;
  if (num_read != 1) {
    return 1;
  }

  char* program_name = (char*)malloc(file_header.program_name_length + 1);
  if (program_name == NULL) {
    return 1;
  }

  num_read = fread(program_name, sizeof(char), file_header.program_name_length,
                   file.get());
  total_bytes += sizeof(char) * file_header.program_name_length;
  if (num_read != file_header.program_name_length) {
    free(program_name);
    return 1;
  }
  program_name[file_header.program_name_length] = 0;
  file_header.program_name = program_name;

  num_read =
      fread(&file_header.program_command_length, sizeof(size_t), 1, file.get());
  total_bytes += sizeof(size_t) * 1;
  if (num_read != 1) {
    free(program_name);
    return 1;
  }

  char* program_command = (char*)malloc(file_header.program_command_length + 1);
  if (program_command == NULL) {
    free(program_name);
    return 1;
  }

  num_read = fread(program_command, sizeof(char),
                   file_header.program_command_length, file.get());
  total_bytes += sizeof(char) * file_header.program_command_length;
  if (num_read != file_header.program_command_length) {
    free(program_name);
    free(program_command);
    return 1;
  }
  program_command[file_header.program_command_length] = 0;
  file_header.program_command = program_command;

  num_read =
      fread(&file_header.program_load_addr, sizeof(intptr_t), 1, file.get());
  total_bytes += sizeof(intptr_t) * 1;
  if (num_read != 1) {
    free(program_name);
    free(program_command);
    return 1;
  }

  std::cerr << "Read a header of " << total_bytes << " bytes.\n";

  return 0;
}

void EventFileReader::read_next_chunk() {
  if (done()) {
    return;
  }
  std::cerr << "sizeof(SyanEvent) in executable: " << sizeof(::SyanEvent)
            << "\n";
  auto num_read =
      std::fread(buffer.get(), sizeof(::SyanEvent), buffer_cap, file.get());
  if (num_read < buffer_cap || std::feof(file.get())) {
    buffer_cap = num_read;
    is_done_reading_file = true;
    file.reset();
  }
  buffer_cursor = 0;
}

void EventFileReader::CloseFileDeleter::operator()(
    std::FILE* f) const noexcept {
  std::fclose(f);
}

}  // namespace syan
