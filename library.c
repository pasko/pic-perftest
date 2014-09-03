/* TODO: put license here. */

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <time.h>

int Helper();

int64_t TimeDiffMicros(const struct timespec* start, const struct timespec* end) {
  int64_t sec_diff = end->tv_sec - start->tv_sec;
  assert(sec_diff >= 0);
  return 1000000 * sec_diff + (end->tv_nsec - start->tv_nsec) / 1000;
}

void __attribute__((weak)) __gcov_dump_all();

void __attribute__ ((visibility ("default"))) RunBenchmark() {
  const size_t kNumIterations = 1024 * 1024 * 256;
  struct timespec start, end;

  clock_gettime(CLOCK_MONOTONIC, &start);

  for (size_t i = 0; i < kNumIterations; i++) {
    if (!Helper(i)) {
      break;
    }
  }

  clock_gettime(CLOCK_MONOTONIC, &end);
  int64_t time_millis = TimeDiffMicros(&start, &end) / 1000;

  printf("PASSED (%" PRId64 "ms)\n", time_millis);
  if (__gcov_dump_all) {
    printf("Writing profile to disk...\n");
    __gcov_dump_all();
    printf("Profile written.\n");
  } else {
    printf("No profile information to dump.\n");
  }
}
