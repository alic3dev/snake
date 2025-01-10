#ifndef __MODE_EXIT_H
#define __MODE_EXIT_H

#include "display.h"
#include "micro_time.h"
#include "mode.h"

struct mode_exit {
  struct display* display;
  struct position offset;
  unsigned char dot_count;
  micro_time micro_time_dot_previous;
  micro_time micro_time_dot_speed;
  unsigned char started_exiting;
};

extern const unsigned char mode_exit_text[];
extern const size_t mode_exit_text_length;

extern const size_t mode_exit_dot_count_max;

void mode_exit_initialize(
  struct mode_exit*,
  struct display*
);
enum MODE mode_exit_poll(
  struct mode_exit*,
  micro_time
);
void mode_exit_display(struct mode_exit*);
void mode_exit_destroy(struct mode_exit*);

#endif

