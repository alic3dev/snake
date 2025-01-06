#ifndef __PLACE_APPLE_H
#define __PLACE_APPLE_H

#include <stddef.h>

#include "position.h"

struct position* place_apple(
  struct position**,
  size_t,
  struct position*
);

#endif

