#ifndef __MODE_INTRO_H
#define __MODE_INTRO_H

#include "micro_time.h"
#include "mode.h"
#include "display.h"

struct mode_intro {
  struct display* display;
  micro_time micro_time_previous;
  micro_time speed;
  size_t progress;
  unsigned char** buffer;
  size_t x_offset;
  size_t y_offset;
};

void mode_intro_initialize(
  struct mode_intro*,
  struct display*
);
enum MODE mode_intro_poll(
  struct mode_intro*,
  micro_time
);
void mode_intro_display(struct mode_intro*);
void mode_intro_destroy(struct mode_intro*);

#endif

