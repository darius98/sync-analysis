#include "dump_file_header.h"

#include <stdio.h>

#include "event_time.h"

#ifdef SYNC_ANALYSIS_IS_MAC_OS_X

#include <dlfcn.h>

static int syan_get_load_addr(intptr_t* load_addr) {
  Dl_info info;
  int status = dladdr(&syan_init_dump_file_header, &info);
  if (status == 0) {
    fprintf(
        stderr,
        "Sync analysis init: dladdr did not return info for a known symbol.\n");
    return 1;
  }
  *load_addr = (intptr_t)info.dli_fbase;
  return 0;
}

#endif

int syan_init_dump_file_header(SyanDumpFileHeader* header) {
  syan_event_time_start(&header->start_time);

#ifdef SYNC_ANALYSIS_IS_MAC_OS_X
  int get_load_addr_status = syan_get_load_addr(&header->program_load_addr);
  if (get_load_addr_status != 0) {
    return get_load_addr_status;
  }
#endif

  return 0;
}
