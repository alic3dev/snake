#ifndef __MODE_GAME_H
#define __MODE_GAME_H

#include "direction.h"
#include "micro_time.h"
#include "mode.h"
#include "display.h"
#include "score.h"

struct mode_game {
  struct display* display;
  size_t snake_length;
  struct position** snake;
  struct position* snake_position_buffer;
  struct position* snake_position_previous;
  enum direction snake_direction;
  enum direction snake_direction_previous;
  micro_time snake_speed;
  micro_time time_movement_previous;
  unsigned char collided;
  struct position* apple_position;
  struct score* score;
  micro_time time_start;
};

void mode_game_initialize(
  struct mode_game*,
  struct display*
);
enum MODE mode_game_poll(
  struct mode_game*,
  micro_time
);
void mode_game_display(struct mode_game*);
void mode_game_destroy(struct mode_game*);

#endif

