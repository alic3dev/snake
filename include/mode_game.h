#ifndef __MODE_GAME_H
#define __MODE_GAME_H

#include "micro_time.h"
#include "mode.h"
#include "display.h"

struct mode_game {
};

void mode_game_initialize(struct mode_game*);
enum MODE mode_game_poll(
  struct mode_game*,
  micro_time
);
void mode_game_display(
  struct mode_game*,
  struct display*
);
void mode_game_destroy(struct mode_game*);

#endif

