#include "print_usage.h"

#include <stdio.h>

void print_usage(unsigned char error) {
  FILE* output_stream;

  if (error == 1) {
    output_stream = stderr;
  } else {
    output_stream = stdout;
  }

  fprintf(
    output_stream,
    "USAGE: snake [options...]\n  --skip-intro\t\tSkips the intro scene\n  --seed [#seed]\tSets the game seed\n"
  );
}

