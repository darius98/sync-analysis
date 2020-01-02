#include "buffer.h"

#include <stdlib.h>

BufferInitStatus syan_buffer_init(Buffer** buffer) {
  *buffer = malloc(sizeof(Buffer));
  if (*buffer == NULL) {
    return BUFFER_INIT_MALLOC_FAILED_BUFFER;
  }

  // TODO

  return BUFFER_INIT_OK;
}

// TODO
