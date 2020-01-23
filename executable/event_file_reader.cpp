#include "event_file_reader.hpp"

#include <stdexcept>

namespace syan {

EventFileReader::EventFileReader(std::string fn, std::size_t buffer_cap)
    : file_name(std::move(fn)), file(std::fopen(file_name.c_str(), "rb")),
      buffer_cap(buffer_cap), buffer_cursor(buffer_cap),
      is_done_reading_file(false) {
  auto buf = std::make_unique<::SyanEvent[]>(buffer_cap);
  if (file == nullptr) {
    throw std::runtime_error("Cannot read dump file at '" + file_name + "'");
  }
  buffer = std::move(buf);
  int read_status = syan_read_dump_file_header(file.get(), &file_header);
  if (read_status != 0) {
    throw std::runtime_error("cannot read dump file at '" + file_name + "'");
  }
  read_next_chunk();
}

DumpFileHeader EventFileReader::get_header() const noexcept {
  return file_header;
}

bool EventFileReader::done() const noexcept {
  return is_done_reading_file && buffer_cursor == buffer_cap;
}

::SyanEvent EventFileReader::read() {
  if (done()) {
    throw std::runtime_error("File " + file_name + " is done.");
  }
  if (buffer_cursor == buffer_cap) {
    read_next_chunk();
  }
  return buffer[buffer_cursor++];
}

void EventFileReader::read_next_chunk() {
  if (done()) {
    throw std::runtime_error("File " + file_name + " is done reading.");
  }
  auto num_read =
      std::fread(buffer.get(), sizeof(::SyanEvent), buffer_cap, file.get());
  if (num_read < buffer_cap || std::feof(file.get())) {
    buffer_cap = num_read;
    is_done_reading_file = true;
    file.reset();
  }
  buffer_cursor = 0;
}

void EventFileReader::CloseFileDeleter::operator()(std::FILE* f) const
    noexcept {
  std::fclose(f);
}

} // namespace syan
