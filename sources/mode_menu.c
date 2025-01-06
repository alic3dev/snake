#include "mode_menu.h"

#include "display.h"
#include "micro_time.h"
#include "mode.h"

void mode_menu_initialize(
  struct mode_menu* mode_menu,
  struct display* display
) {
  mode_menu->display = display;
}

enum MODE mode_menu_poll(
  struct mode_menu* mode_menu,
  micro_time current_micro_time
) {
  return GAME;
}

void mode_menu_display(
  struct mode_menu* mode_menu
) {
}

void mode_menu_destroy(
  struct mode_menu* mode_menu
) {
}

