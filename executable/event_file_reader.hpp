#ifndef SYNC_ANALYSIS_EXE_EVENT_FILE_READER_H_
#define SYNC_ANALYSIS_EXE_EVENT_FILE_READER_H_

#include <cstdio>

#include <memory>
#include <string>

#include "event.hpp"

namespace syan {

class EventFileReader {
public:
  explicit EventFileReader(std::string file_name, std::size_t buffer_cap = 256);

  bool done() const noexcept;

  Event read();

private:
  void read_next_chunk();

  struct CloseFileDeleter {
    void operator()(FILE* file) const noexcept;
  };

  std::string file_name;
  std::unique_ptr<std::FILE, CloseFileDeleter> file;
  std::size_t buffer_cap;
  std::size_t buffer_cursor;
  std::unique_ptr<Event[]> buffer;
  bool is_done_reading_file;
};

} // namespace syan

#endif