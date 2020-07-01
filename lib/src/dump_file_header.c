#include "dump_file_header.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "event_time.h"

#include <dlfcn.h>

static int syan_get_load_addr(intptr_t* load_addr) {
  Dl_info info;
  int status = dladdr((void*)&syan_init_dump_file_header, &info);
  if (status == 0) {
    fprintf(
        stderr,
        "Sync analysis init: dladdr did not return info for a known symbol.\n");
    return 1;
  }
  *load_addr = (intptr_t)info.dli_fbase;
  return 0;
}

int syan_init_dump_file_header(SyanDumpFileHeader* header, int argc,
                               char** argv) {
  syan_event_time_start(&header->start_time);

  header->program_name = strrchr(argv[0], '/') + 1;
  header->program_name_length = strlen(header->program_name);

  header->program_command_length = argc - 1;
  for (int i = 0; i < argc; i++) {
    header->program_command_length += strlen(argv[i]);
  }
  char* program_command = malloc(header->program_command_length + 1);
  if (program_command == NULL) {
    return 1;
  }
  program_command[0] = 0;
  strcat(program_command, argv[0]);
  for (int i = 1; i < argc; i++) {
    strcat(program_command, " ");
    strcat(program_command, argv[i]);
  }
  header->program_command = program_command;

  int get_load_addr_status = syan_get_load_addr(&header->program_load_addr);
  if (get_load_addr_status != 0) {
    free(program_command);
    return get_load_addr_status;
  }

  return 0;
}

int syan_write_dump_file_header(FILE* stream,
                                const SyanDumpFileHeader* header) {
  size_t num_written;
  num_written = fwrite(&header->start_time, sizeof(struct timespec), 1, stream);
  if (num_written != 1) {
    return 1;
  }

  num_written = fwrite(&header->program_name_length, sizeof(size_t), 1, stream);
  if (num_written != 1) {
    return 1;
  }

  num_written = fwrite(header->program_name, sizeof(char),
                       header->program_name_length, stream);
  if (num_written != header->program_name_length) {
    return 1;
  }

  num_written =
      fwrite(&header->program_command_length, sizeof(size_t), 1, stream);
  if (num_written != 1) {
    return 1;
  }

  num_written = fwrite(header->program_command, sizeof(char),
                       header->program_command_length, stream);
  if (num_written != header->program_command_length) {
    return 1;
  }

  num_written = fwrite(&header->program_load_addr, sizeof(intptr_t), 1, stream);
  if (num_written != 1) {
    return 1;
  }

  return 0;
}
