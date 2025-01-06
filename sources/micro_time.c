#include <stddef.h>
#include <sys/time.h>

#include "micro_time.h"

micro_time get_micro_time() {
  struct timeval time_val;

  gettimeofday(&time_val, NULL);

  micro_time current_micro_time = (
    (time_val.tv_sec * 1000000) +
    time_val.tv_usec
  );

  return current_micro_time;
}

