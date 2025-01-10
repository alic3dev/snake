#include "mode_intro.h"

#include <stdlib.h>

#include "display.h"
#include "game_logo.h"
#include "micro_time.h"
#include "mode.h"

void mode_intro_initialize(
  struct mode_intro* mode_intro,
  struct display* display
) {
  mode_intro->display = display;
  mode_intro->micro_time_previous = 0;
  mode_intro->speed = 5000;
  mode_intro->progress = 0;
  
  mode_intro->x_offset = (
    (
      mode_intro->display->terminal_size->x -
      game_logo_columns
    ) / 2
  );
  mode_intro->y_offset = (
    (
      mode_intro->display->terminal_size->y -
      game_logo_rows
    ) / 2
  );

  mode_intro->buffer = malloc(
    sizeof(unsigned char*) * game_logo_rows
  );

  for (size_t y = 0; y < game_logo_rows; ++y) {
    mode_intro->buffer[y] = malloc(
      sizeof(unsigned char) * game_logo_columns
    );

    for (
      size_t x = 0;
      x < game_logo_columns;
      ++x
    ) {
      mode_intro->buffer[y][x] = '\0';
    }
  }
}

enum MODE mode_intro_poll(
  struct mode_intro* mode_intro,
  micro_time micro_time_current
) {
  micro_time micro_time_delta = (
    micro_time_current -
    mode_intro->micro_time_previous
  );

  if (micro_time_delta < mode_intro->speed) {
    return MODE_INTRO;
  }
  
  if (
    mode_intro->progress < game_logo_total_size
  ) {
    unsigned char valid_position = 0;
    struct position position;
    
    do {
      position.x = rand() % game_logo_columns;
      position.y = rand() % game_logo_rows;

      if (
        mode_intro->buffer[position.y][position.x]
        != '\0'
      ) {
        continue;
      }

      mode_intro->buffer[
        position.y
      ][
        position.x
      ] = (
        game_logo[position.y][position.x]
      );

      valid_position = 1;
    } while (valid_position != 1);
    
    mode_intro->display->should_render = 1;
  }

  mode_intro->progress += 1;
  mode_intro->micro_time_previous = (
    micro_time_current
  );

  if (
    mode_intro->progress >= 
    game_logo_total_size * 2
  ) {
    return MODE_MENU;
  }
  
  return MODE_INTRO;
}

void mode_intro_display(
  struct mode_intro* mode_intro
) {
  if (mode_intro->display->should_render != 1) {
    return;
  }

  display_use_frame(mode_intro->display);

  for (size_t y = 0; y < game_logo_rows; ++y) {
    size_t y_offset = (
      (mode_intro->display->terminal_size->x + 1)
      * (y + mode_intro->y_offset)
    );

    for (
      size_t x = 0;
      x < game_logo_columns;
      ++x
    ) {
      if (mode_intro->buffer[y][x] == '\0') {
        continue;
      }

      mode_intro->display->buffer[
        y_offset + mode_intro->x_offset + x
      ] = mode_intro->buffer[y][x];
    }
  }
}

void mode_intro_destroy(
  struct mode_intro* mode_intro
) {
  for (size_t i = 0; i < game_logo_rows; ++i) {
    free(mode_intro->buffer[i]);
  }
  free(mode_intro->buffer);
}

