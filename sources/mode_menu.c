#include "mode_menu.h"
#include <stdio.h>
#include <pthread.h>

#include "display.h"
#include "key_values.h"
#include "menu.h"
#include "mode.h"
#include "user_input.h"

void mode_menu_initialize(
  struct mode_menu* mode_menu,
  struct display* display
) {
  mode_menu->display = display;

  menu_initialize(
    &mode_menu->menu,
    mode_menu->display
  );

  menu_add_option(
    &mode_menu->menu,
    "START",
    MENU_ACTION_START
  );

  menu_add_option(
    &mode_menu->menu,
    "EXIT",
    MENU_ACTION_EXIT
  );

  menu_offset_center(
    &mode_menu->menu
  );
}

enum MODE mode_menu_poll(
  struct mode_menu* mode_menu,
  micro_time current_micro_time
) {
  enum menu_action menu_action_selected = (
    menu_poll(&mode_menu->menu)
  );

  switch (menu_action_selected) {
    case MENU_ACTION_START:
      return MODE_GAME;
    case MENU_ACTION_EXIT:
      return MODE_EXIT;
    default:
      break;
  }

  return MODE_MENU;
}

void mode_menu_display(
  struct mode_menu* mode_menu
) {
  if (mode_menu->display->should_render == 1) {
    display_use_frame(mode_menu->display);
    menu_display(&mode_menu->menu);
  }
}

void mode_menu_destroy(
  struct mode_menu* mode_menu
) {
  menu_destroy(&mode_menu->menu);
}

