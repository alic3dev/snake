#ifndef __MODE_GAMEOVER_H
#define __MODE_GAMEOVER_H

#include "display.h"
#include "micro_time.h"
#include "menu.h"
#include "mode.h"
#include "score.h"

struct mode_gameover {
  struct display* display;
  struct score* score;
  struct menu menu;
};

void mode_gameover_initialize(
  struct mode_gameover*,
  struct display*
);
enum MODE mode_gameover_poll(
  struct mode_gameover*,
  micro_time
);
void mode_gameover_display(struct mode_gameover*);
void mode_gameover_destroy(struct mode_gameover*);

#endif

