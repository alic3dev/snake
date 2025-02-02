#include "mode_game.h"

#include <pthread.h>
#include <stdlib.h>

#include "direction.h"
#include "display.h"
#include "key_values.h"
#include "micro_time.h"
#include "mode.h"
#include "place_apple.h"
#include "user_input.h"

void mode_game_initialize(
  struct mode_game* mode_game,
  struct display* display
) {
  mode_game->display = display;

  mode_game->snake_length = 1;
  mode_game->snake = malloc(
    sizeof(struct position*) *
    mode_game->snake_length
  );

  mode_game->snake[0] = malloc(
    sizeof(struct position)
  );
  mode_game->snake[0]->x = (
    (rand() % (
      mode_game->display->terminal_size->x - 2
    )) + 1
  );
  mode_game->snake[0]->y = (
    (rand() % (
      mode_game->display->terminal_size->y - 2
    )) + 1
  );

  mode_game->snake_position_buffer = malloc(
    sizeof(struct position)
  );
  mode_game->snake_position_previous = malloc(
    sizeof(struct position)
  );
  
  mode_game->snake_direction = UP;
  mode_game->snake_direction_previous = DOWN;
  mode_game->time_movement_previous = (
    get_micro_time()
  );
  mode_game->snake_speed = 100000;
  mode_game->collided = 0;
  
  mode_game->apple_position = place_apple(
    mode_game->snake,
    mode_game->snake_length,
    mode_game->display->terminal_size
  );

  mode_game->score->score = 0;
  mode_game->score->apples_eaten = 0;
  mode_game->score->total_time = 0;

  mode_game->time_start = get_micro_time();
}

enum MODE mode_game_poll(
  struct mode_game* mode_game,
  micro_time micro_time_current
) {
  pthread_mutex_lock(&user_input_mutex);
  switch (user_input) {
    case KEY_VALUE_ARROW_UP:
    case KEY_VALUE_W:
    case KEY_VALUE_I:
      if (
        mode_game->snake_direction_previous !=
        DOWN
      ) {
        mode_game->snake_direction = UP;
      }
      break;
    case KEY_VALUE_ARROW_DOWN:
    case KEY_VALUE_S:
    case KEY_VALUE_K:
      if (
        mode_game->snake_direction_previous != UP
      ) {
        mode_game->snake_direction = DOWN;
      }
      break;
    case KEY_VALUE_ARROW_RIGHT:
    case KEY_VALUE_D:
    case KEY_VALUE_L:
      if (
        mode_game->snake_direction_previous !=
        LEFT
      ) {
        mode_game->snake_direction = RIGHT;
      }
      break;
    case KEY_VALUE_ARROW_LEFT:
    case KEY_VALUE_A:
    case KEY_VALUE_J:
      if (
        mode_game->snake_direction_previous !=
        RIGHT
      ) {
        mode_game->snake_direction = LEFT;
      }
      break;
    default:
      break;
  }
  user_input = -1;
  pthread_mutex_unlock(&user_input_mutex);

  micro_time time_movement_delta = (
    micro_time_current -
    mode_game->time_movement_previous
  );

  if (
    time_movement_delta >= mode_game->snake_speed
  ) {
    mode_game->snake_position_previous->x = (
      mode_game->snake[0]->x
    );
    mode_game->snake_position_previous->y = (
      mode_game->snake[0]->y
    );

    switch (mode_game->snake_direction) {
      case UP:
        if (mode_game->snake[0]->y == 1) {
          mode_game->collided = 1;
        } else {
          mode_game->snake[0]->y -= 1;  
        }
        break;
      case DOWN:
        if (
          mode_game->snake[0]->y ==
          mode_game->display->terminal_size->y - 2
        ) {
          mode_game->collided = 1;
        } else {
          mode_game->snake[0]->y += 1;  
        }
        break;
      case RIGHT:
        if (
          mode_game->snake[0]->x == 
          mode_game->display->terminal_size->x - 2
        ) {
          mode_game->collided = 1;
        } else {
          mode_game->snake[0]->x += 1;  
        }
        break;
      case LEFT:
        if (mode_game->snake[0]->x == 1) {
          mode_game->collided = 1;
        } else {
          mode_game->snake[0]->x -= 1;  
        }
        break;
      default:
        break;
    }

    for (
      size_t i = 1;
      i < mode_game->snake_length;
      ++i
    ) {
      if (
        mode_game->snake[0]->x == 
        mode_game->snake[i]->x &&
        mode_game->snake[0]->y == 
        mode_game->snake[i]->y
      ) {
        mode_game->collided = 1;
        break;
      }
    }

    if (mode_game->collided == 0) {
      for (
        size_t i = 1;
        i < mode_game->snake_length;
        ++i
      ) {
        mode_game->snake_position_buffer->x = (
          mode_game->snake[i]->x
        );
        mode_game->snake_position_buffer->y = (
          mode_game->snake[i]->y
        );
        mode_game->snake[i]->x = (
          mode_game->snake_position_previous->x
        );
        mode_game->snake[i]->y = (
          mode_game->snake_position_previous->y
        );
        mode_game->snake_position_previous->x = (
          mode_game->snake_position_buffer->x
        );
        mode_game->snake_position_previous->y = (
          mode_game->snake_position_buffer->y
        );
      }

      if (
        mode_game->snake[0]->x == 
        mode_game->apple_position->x &&
        mode_game->snake[0]->y == 
        mode_game->apple_position->y
      ) {
        mode_game->snake_length += 1;
        mode_game->snake = realloc(
          mode_game->snake,
          sizeof(struct position*) * 
            mode_game->snake_length
        );

        mode_game->snake[
          mode_game->snake_length - 1
        ] = (
          malloc(sizeof(struct position))
        );
        mode_game->snake[
          mode_game->snake_length - 1
        ]->x = (
          mode_game->snake_position_previous->x
        );
        mode_game->snake[
          mode_game->snake_length - 1
        ]->y = (
          mode_game->snake_position_previous->y
        );

        if (mode_game->apple_position != NULL) {
          free(mode_game->apple_position);
        }

        mode_game->score->score = (
          mode_game->score->score +
          100 +
          (mode_game->score->apples_eaten * 10)
        );

        mode_game->score->apples_eaten = (
          mode_game->score->apples_eaten + 1
        ); 

        mode_game->apple_position = place_apple(
          mode_game->snake,
          mode_game->snake_length,
          mode_game->display->terminal_size
        );
      }
    } else {
      mode_game->score->total_time = (
        get_micro_time() -
        mode_game->time_start
      );
      return MODE_GAMEOVER;
    }

    mode_game->display->should_render = 1;

    mode_game->snake_direction_previous = (
      mode_game->snake_direction
    );
 
    mode_game->time_movement_previous = (
      micro_time_current
    );
  }

  return MODE_GAME;
}

void mode_game_display(
  struct mode_game* mode_game
) {
  if (mode_game->display->should_render == 0) {
    return;
  }

  display_use_frame(mode_game->display);

  for (
    size_t i = 0;
    i < mode_game->snake_length;
    ++i
  ) {
    size_t y_offset = (
      mode_game->snake[i]->y * (
        mode_game->display->terminal_size->x +
        1
      )
    );

    mode_game->display->buffer[
      mode_game->snake[i]->x + y_offset
    ] = i == 0 ? '0' : '8';
  }

  if (mode_game->apple_position != NULL) {
    size_t y_offset = (
      mode_game->apple_position->y * (
        mode_game->display->terminal_size->x +
        1
      )
    );

    mode_game->display->buffer[
      mode_game->apple_position->x + y_offset
    ] = '@';
  }
}

void mode_game_destroy(
  struct mode_game* mode_game
) {
  for (
    size_t i = 0;
    i < mode_game->snake_length;
    ++i
  ) {
    free(mode_game->snake[i]);
  }
  free(mode_game->snake);

  free(mode_game->snake_position_buffer);
  free(mode_game->snake_position_previous);

  if (mode_game->apple_position != NULL) {
    free(mode_game->apple_position);
  }
}

