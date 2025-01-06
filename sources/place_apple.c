#include "place_apple.h"

#include <stddef.h>
#include <stdlib.h>

#include "position.h"

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

