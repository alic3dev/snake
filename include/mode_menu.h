#ifndef __MODE_MENU_H
#define __MODE_MENU_H

#include "display.h"
#include "menu.h"
#include "micro_time.h"
#include "mode.h"

struct mode_menu {
  struct display* display;
  struct menu menu;
};

void mode_menu_initialize(
  struct mode_menu*,
  struct display*
);

enum MODE mode_menu_poll(
  struct mode_menu*,
  micro_time
);

void mode_menu_display(struct mode_menu*);

void mode_menu_destroy(struct mode_menu*);

#endif

