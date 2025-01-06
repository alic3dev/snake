#include "mode_functions.h"

#include "display.h"
#include "micro_time.h"
#include "mode.h"
#include "mode_game.h"
#include "mode_intro.h"
#include "mode_menu.h"

void mode_initialize(
  enum MODE mode,
  void* mode_struct
) {
  switch (mode) {
    case GAME:
      return mode_game_initialize(
        (struct mode_game*) mode_struct
      );
    case INTRO:
      return mode_intro_initialize(
        (struct mode_intro*) mode_struct
      );
    case MENU:
      return mode_menu_initialize(
        (struct mode_menu*) mode_struct
      );
    default:
      break;
  }
}

enum MODE mode_poll(
  enum MODE mode,
  void* mode_struct,
  micro_time current_micro_time
) {
  switch (mode) {
    case GAME:
      return mode_game_poll(
        (struct mode_game*) mode_struct,
        current_micro_time
      );
    case INTRO:
      return mode_intro_poll(
        (struct mode_intro*) mode_struct,
        current_micro_time
      );
    case MENU:
      return mode_menu_poll(
        (struct mode_menu*) mode_struct,
        current_micro_time
      );
    default:
      break;
  }

  return INTRO;
}

void mode_display(
  enum MODE mode,
  void* mode_struct,
  struct display* display
) {
  switch (mode) {
    case GAME:
      return mode_game_display(
        (struct mode_game*) mode_struct,
        display
      );
    case INTRO:
      return mode_intro_display(
        (struct mode_intro*) mode_struct,
        display
      );
    case MENU:
      return mode_menu_display(
        (struct mode_menu*) mode_struct,
        display
      );
    default:
      break;
  }
}


void mode_destroy(
  enum MODE mode,
  void* mode_struct
) {
  switch (mode) {
    case GAME:
      return mode_game_destroy(
        (struct mode_game*) mode_struct
      );
    case INTRO:
      return mode_intro_destroy(
        (struct mode_intro*) mode_struct
      );
    case MENU:
      return mode_menu_destroy(
        (struct mode_menu*) mode_struct
      );
    default:
      break;
  }
}

