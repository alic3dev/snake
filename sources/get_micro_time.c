#include <stddef.h>
#include <sys/time.h>

#include "get_micro_time.h"

unsigned long long int get_micro_time() {
  struct timeval time_val;

  gettimeofday(&time_val, NULL);

  unsigned long long micro_time = (
    (time_val.tv_sec * 1000000) +
    time_val.tv_usec
  );

  return micro_time;
}

