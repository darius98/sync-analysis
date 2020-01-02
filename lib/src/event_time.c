#include "event_time.h"

#include <errno.h>
#include <time.h>

static struct timespec start_time;

int syan_event_time_init() {
  if (clock_gettime(CLOCK_MONOTONIC, &start_time) != 0) {
    return errno;
  }
  return 0;
}

int64_t syan_event_time_now() {
  struct timespec current_time;
  clock_gettime(CLOCK_MONOTONIC, &current_time);
  if (current_time.tv_nsec < start_time.tv_nsec) {
    current_time.tv_nsec += 1000000000;
    current_time.tv_sec -= 1;
  }
  return ((int64_t)1000000000) * (current_time.tv_sec - start_time.tv_sec) +
         (current_time.tv_nsec - start_time.tv_nsec);
}
