#ifndef __SCORE_H
#define __SCORE_H

#include "micro_time.h"

struct score {
  long int score;
  unsigned int apples_eaten;
  micro_time total_time;
};

#endif

