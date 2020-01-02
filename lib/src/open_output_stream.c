#include "open_output_stream.h"

#include <fcntl.h>

int syan_open_output_stream() {
  return open("syan_report.dump", O_CREAT | O_WRONLY | O_TRUNC,
              S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
}
