#ifndef __DISPLAY_H
#define __DISPLAY_H

#include <stddef.h>

#include "position.h"

struct display {
  unsigned char *buffer;
  unsigned char *buffer_frame;
  unsigned char *buffer_empty;
  size_t buffer_length;
  unsigned char should_render;
};

void display_initialize(
  struct display*,
  struct position*
);

void display_use_empty(struct display*);
void display_use_frame(struct display*);

void display_print(struct display*);

void display_destroy(struct display*);

#endif

