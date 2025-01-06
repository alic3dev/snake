#ifndef __OPTIONS_H
#define __OPTIONS_H

struct options {
  unsigned long long int seed;
  unsigned char skip_intro;
};

unsigned char options_initialize(
  struct options*,
  int,
  char**
);

#endif

