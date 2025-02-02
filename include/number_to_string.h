#ifndef __NUMBER_TO_STRING_H
#define __NUMBER_TO_STRING_H

#include "micro_time.h"

char* number_to_string_li(
  long int number
);

char* number_to_string_ui(
  unsigned int number
);

char* number_to_string_mt(
  micro_time number
);

#endif

