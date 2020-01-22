#include <errno.h>
#include <time.h>

static struct timespec syan_start_time;
static struct timespec syan_real_start_time;

int syan_event_time_init() {
  if (clock_gettime(CLOCK_MONOTONIC, &syan_start_time) != 0) {
    return errno;
  }
  if (clock_gettime(CLOCK_REALTIME, &syan_real_start_time) != 0) {
    return errno;
  }
  return 0;
}

void syan_event_time_start(struct timespec* spec) {
  (*spec) = syan_real_start_time;
}

int64_t syan_event_time_now() {
  struct timespec current_time;
  clock_gettime(CLOCK_MONOTONIC, &current_time);
  if (current_time.tv_nsec < syan_start_time.tv_nsec) {
    current_time.tv_nsec += 1000000000;
    current_time.tv_sec -= 1;
  }
  return ((int64_t)1000000000) *
             (current_time.tv_sec - syan_start_time.tv_sec) +
         (current_time.tv_nsec - syan_start_time.tv_nsec);
}
