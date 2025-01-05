#include <pthread.h>
#include <stddef.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <termios.h>
#include <unistd.h>

#include "snake.h"
#include "direction.h"
#include "display.h"
#include "game_logo.h"
#include "get_micro_time.h"
#include "key_values.h"
#include "position.h"
#include "terminal_size.h"

unsigned char keep_running = 1;
unsigned char keep_running_lockable = 1;
pthread_mutex_t keep_running_mutex;

int user_input = -1;
pthread_mutex_t user_input_mutex;

#ifndef __SNAKE_C_VALID_INPUTS_LENGTH
#define __SNAKE_C_VALID_INPUTS_LENGTH 12
#endif
int valid_inputs[__SNAKE_C_VALID_INPUTS_LENGTH] = {
  KEY_VALUE_ARROW_UP,
  KEY_VALUE_ARROW_DOWN,
  KEY_VALUE_ARROW_RIGHT,
  KEY_VALUE_ARROW_LEFT,
  KEY_VALUE_W,
  KEY_VALUE_S,
  KEY_VALUE_D,
  KEY_VALUE_A,
  KEY_VALUE_I,
  KEY_VALUE_K,
  KEY_VALUE_L,
  KEY_VALUE_J
};

int main() {
  srand(get_micro_time());

  struct sigaction signal_action;
  signal_action.sa_handler = interupt_handler;
  sigaction(SIGINT, &signal_action, NULL);

  struct position* terminal_size = get_terminal_size();
  terminal_size->y -= 1;

  if (terminal_size == NULL) {
    fprintf(stderr, "Couldn't determine terminal size\n");
    return 1;
  }

  struct display* display;
  display = malloc(sizeof(struct display));
  display_initialize(display, terminal_size);

  size_t snake_length = 1;
  struct position** snake;
  snake = malloc(sizeof(struct position*) * snake_length);
  snake[0] = malloc(sizeof(struct position));
  snake[0]->x=(rand() % (terminal_size->x - 2)) + 1;
  snake[0]->y=(rand() % (terminal_size->y - 2)) + 1;
  struct position* snake_position_buffer;
  snake_position_buffer = malloc(sizeof(struct position));
  struct position* snake_position_previous;
  snake_position_previous = malloc(sizeof(struct position));
  enum direction snake_direction = UP;
  enum direction snake_direction_previous = DOWN;
  unsigned long long int snake_speed = 100000;
  unsigned char collided = 0;

  struct position* apple_position;
  apple_position = place_apple(
    snake,
    snake_length,
    terminal_size
  );

  pthread_mutex_init(&keep_running_mutex, NULL);
  pthread_mutex_init(&user_input_mutex, NULL);

  pthread_t input_thread;
  pthread_create(
    &input_thread, 
    NULL,
    get_user_input,
    NULL
  );

  unsigned long long int time_snake_previous = (
    get_micro_time()
  );

  while (keep_running) {
    pthread_mutex_lock(&user_input_mutex);
    switch (user_input) {
      case KEY_VALUE_ARROW_UP:
      case KEY_VALUE_W:
      case KEY_VALUE_I:
        if (snake_direction_previous != DOWN) {
          snake_direction = UP;
        }
        break;
      case KEY_VALUE_ARROW_DOWN:
      case KEY_VALUE_S:
      case KEY_VALUE_K:
        if (snake_direction_previous != UP) {
          snake_direction = DOWN;
        }
        break;
      case KEY_VALUE_ARROW_RIGHT:
      case KEY_VALUE_D:
      case KEY_VALUE_L:
        if (snake_direction_previous != LEFT) {
          snake_direction = RIGHT;
        }
        break;
      case KEY_VALUE_ARROW_LEFT:
      case KEY_VALUE_A:
      case KEY_VALUE_J:
        if (snake_direction_previous != RIGHT) {
          snake_direction = LEFT;
        }
        break;
      default:
        break;
    }

    user_input = -1;
    pthread_mutex_unlock(&user_input_mutex);

    unsigned long long int time_current = (
      get_micro_time()
    );
    unsigned long long int time_snake_delta = (
      time_current - time_snake_previous
    );

    if (time_snake_delta >= snake_speed) {
      snake_position_previous->x = snake[0]->x;
      snake_position_previous->y = snake[0]->y;

      switch (snake_direction) {
        case UP:
          if (snake[0]->y == 1) {
            collided = 1;
          } else {
            snake[0]->y -= 1;  
          }
          break;
       case DOWN:
          if (snake[0]->y == terminal_size->y - 2) {
            collided = 1;
          } else {
            snake[0]->y += 1;  
          }
          break;
        case RIGHT:
          if (snake[0]->x == terminal_size->x - 2) {
            collided = 1;
          } else {
            snake[0]->x += 1;  
          }
          break;
        case LEFT:
          if (snake[0]->x == 1) {
            collided = 1;
          } else {
            snake[0]->x -= 1;  
          }
          break;
        default:
          break;
      }

      for (size_t i = 1; i < snake_length; i++) {
        if (
          snake[0]->x == snake[i]->x &&
          snake[0]->y == snake[i]->y
        ) {
          collided = 1;
          break;
        }
      }

      if (collided == 0) {
        for (size_t i = 1; i < snake_length; i++) {
          snake_position_buffer->x = snake[i]->x;
          snake_position_buffer->y = snake[i]->y;
          snake[i]->x = snake_position_previous->x;
          snake[i]->y = snake_position_previous->y;
          snake_position_previous->x = snake_position_buffer->x;
          snake_position_previous->y = snake_position_buffer->y;
        }

        if (
          snake[0]->x == apple_position->x &&
          snake[0]->y == apple_position->y
        ) {
          snake_length += 1;
          snake = realloc(
            snake,
            sizeof(struct position*) *
            snake_length
          );

          snake[snake_length - 1] = malloc(sizeof(struct position));
          snake[snake_length - 1]->x = snake_position_previous->x;
          snake[snake_length - 1]->y = snake_position_previous->y;

          if (apple_position != NULL) {
            free(apple_position);
          }

          apple_position = place_apple(
            snake,
            snake_length,
            terminal_size
          );
        }
      }
      display->should_render = 1;

      snake_direction_previous = snake_direction;
      time_snake_previous = time_current;
    }

    if (display->should_render) {
      display_use_frame(display);

      for (size_t i = 0; i < snake_length; ++i) {
        display->buffer[
          snake[i]->x +
          (snake[i]->y * (terminal_size->x + 1))
        ] = i == 0 ? '0' : '8';
      }

      if (apple_position != NULL) {
        display->buffer[
          apple_position->x +
          (apple_position->y * (terminal_size->x + 1))
        ] = '@';
      }
    }

    display_print(display);
  }

  printf("Press any key to exit\n");

  pthread_join(input_thread, NULL);
  pthread_mutex_destroy(&keep_running_mutex);
  pthread_mutex_destroy(&user_input_mutex);

  free(terminal_size);

  display_destroy(display);
  free(display);

  for(size_t i = 0; i < snake_length; i++){
    free(snake[i]);
  }
  free(snake);
  free(snake_position_buffer);
  free(snake_position_previous);

  if (apple_position != NULL) {
    free(apple_position);
  }

  return 0;
}

void* get_user_input() { 
  struct termios termios_attrs_original;
  struct termios termios_attrs_updated;
  
  tcgetattr(
    STDIN_FILENO,
    &termios_attrs_original
  );

  tcgetattr(
    STDIN_FILENO,
    &termios_attrs_updated
  );

  termios_attrs_updated.c_lflag &= ~(
    ICANON | ECHO
  );
  
  tcsetattr(
    STDIN_FILENO,
    TCSANOW,
    &termios_attrs_updated
  );

  int _user_input;

  do {
    pthread_mutex_unlock(&keep_running_mutex);

    _user_input = getchar();

    for (
      size_t i = 0;
      i < __SNAKE_C_VALID_INPUTS_LENGTH; 
      i++
    ) {
      if (_user_input == valid_inputs[i]) {
        pthread_mutex_lock(&user_input_mutex);
        user_input = _user_input;
        pthread_mutex_unlock(&user_input_mutex);
        break;
      }
    }

    pthread_mutex_lock(&keep_running_mutex);
  } while (keep_running_lockable == 1);
 
  tcsetattr(
    STDIN_FILENO,
    TCSANOW,
    &termios_attrs_original
  );
}

void interupt_handler(int _) {
  keep_running = 0;
  pthread_mutex_lock(&keep_running_mutex);
  keep_running_lockable = 0;
  pthread_mutex_unlock(&keep_running_mutex);
}

struct position* place_apple(
  struct position** snake,
  size_t snake_length,
  struct position* terminal_size
) {
  size_t total_size = (
    (terminal_size->y - 3) *
    (terminal_size->x - 2)
  );

  struct position** attempted_positions;
  size_t attempted_positions_length = 0;
  attempted_positions = malloc(
    sizeof(struct position*) *
    attempted_positions_length
  );

  size_t iterations = 0;
  unsigned char placed = 0;

  unsigned char is_invalid_position = 0;

  static struct position* apple_position;
  apple_position = malloc(sizeof(struct position));

  do {
    is_invalid_position = 0;

    apple_position->x = (rand() % (terminal_size->x - 3)) + 1;
    apple_position->y = (rand() % (terminal_size->y - 2)) + 1;

    for (
      size_t i = 0;
      i < attempted_positions_length;
      ++i
    ) {
      if (
        apple_position->x == attempted_positions[i]->x &&
        apple_position->y == attempted_positions[i]->y
      ) {
        is_invalid_position = 1;
        break;
      }
    }

    for (
      size_t i = 0;
      i < snake_length;
      i++
    ) {
      if (
        apple_position->x == snake[i]->x &&
        apple_position->y == snake[i]->y
      ) {
        is_invalid_position = 1;
        break;
      }
    }

    attempted_positions_length += 1;
    attempted_positions = realloc(
      attempted_positions, 
      sizeof(struct position*) * attempted_positions_length
    );

    attempted_positions[
      attempted_positions_length - 1
    ] = malloc(sizeof(struct position));

    attempted_positions[
      attempted_positions_length - 1
    ]->x = apple_position->x;
    attempted_positions[
      attempted_positions_length - 1
    ]->y = apple_position->y;

    if (is_invalid_position == 0) {
      placed = 1;
    } else {
      iterations++;
    }
  } while(
    iterations < total_size &&
    placed == 0
  );

  for (
    size_t i = 0;
    i < attempted_positions_length;
    i++
  ) {
    free(attempted_positions[i]);
  }
  free(attempted_positions);

  if (placed == 1) {
    return apple_position;
  } else {
    free(apple_position);
    return NULL;
  }
}

