#include "options.h"

#include <stdlib.h>
#include <string.h>

#include "micro_time.h"

unsigned char options_initialize(
  struct options* options,
  int argc,
  char** argv
) {
  options->seed = get_micro_time();
  options->skip_intro = 0;

  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "--skip-intro") == 0) {
      options->skip_intro = 1;
    } else if (strcmp(argv[i], "--seed") == 0) {
      if (i == argc - 1) {
        return 1;
      }

      ++i;

      size_t length = strlen(argv[i]);
      
      for (size_t c = 0; c < length; ++c) {
        if (
          argv[i][c] < '0' || argv[i][c] > '9'
        ) {
          return 1;
        }
      }

      options->seed = (
        (unsigned long long int) atoll(argv[i])
      );
    } else {
      return 1;
    }
  }

  return 0;
}

