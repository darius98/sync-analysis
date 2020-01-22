#include "dump_file_header.h"

#include "event_time.h"

int syan_init_dump_file_header(SyanDumpFileHeader* header) {
  syan_event_time_start(&header->start_time);
  return 0;
}
