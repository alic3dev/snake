#include "mode_game.h"

#include "display.h"
#include "micro_time.h"
#include "mode.h"

void mode_game_initialize(
  struct mode_game* mode_game
) {
}

enum MODE mode_game_poll(
  struct mode_game* mode_game,
  micro_time current_micro_time
) {
  return GAME;
}

void mode_game_display(
  struct mode_game* mode_game,
  struct display* display
) {
}

void mode_game_destroy(
  struct mode_game* mode_game
) {
}

