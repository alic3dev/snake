#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "display.h"
#include "position.h"

void display_initialize(
  struct display* display,
  struct position* terminal_size
) {
  display->buffer_length = (
    (terminal_size->x + 1) * terminal_size->y + 1
  );
  display->buffer = malloc(
    sizeof(unsigned char) * display->buffer_length
  );
  display->buffer_frame = malloc(
    sizeof(unsigned char) * display->buffer_length
  );
  display->buffer_empty = malloc(
    sizeof(unsigned char) * display->buffer_length
  );

  for (
    size_t i = 0;
    i < display->buffer_length;
    ++i
  ) {
    display->buffer_empty[i] = ' ';
  }

  display->buffer_empty[
    display->buffer_length - 1
  ] = '\0';

  memcpy(
    display->buffer,
    display->buffer_empty,
    sizeof(unsigned char) * display->buffer_length
  );

  memcpy(
    display->buffer_frame,
    display->buffer_empty,
    sizeof(unsigned char) * display->buffer_length
  );

  for (size_t y = 0; y < terminal_size->y; ++y) {
    size_t y_offset = y * (terminal_size->x + 1);

    for (
      size_t x = 0;
      x <= terminal_size->x;
      ++x
    ) {
      size_t i = y_offset + x;

      if (x == terminal_size->x) {
        display->buffer_frame[i] = '\n';
      } else if (x == terminal_size->x - 1) {
        if (y == 0) {
          display->buffer_frame[i] = '\\';
        } else if (y == terminal_size->y - 1) {
          display->buffer_frame[i] = '/';
        } else {
          display->buffer_frame[i] = '|';
        }
      } else if (x == 0) {
        if (y == 0) {
          display->buffer_frame[i] = '/';
        } else if (y == terminal_size->y - 1) {
          display->buffer_frame[i] = '\\';
        } else {
          display->buffer_frame[i] = '|';
        }
      } else if (y == 0 || y == terminal_size->y - 1) {
        display->buffer_frame[i] = '-';
      }
    }
  }

  display->should_render = 1;
}

void display_use_frame(struct display* display) {
  memcpy(
    display->buffer,
    display->buffer_frame,
    sizeof(unsigned char) * display->buffer_length
  );
}

void display_use_empty(struct display* display) {
  memcpy(
    display->buffer,
    display->buffer_empty,
    sizeof(unsigned char) * display->buffer_length
  );
}

void display_print(struct display* display) {
  if (display->should_render == 1) {
    printf("\r\n%s", display->buffer);
    display->should_render = 0;
  }
}

void display_destroy(struct display* display) {
  free(display->buffer);
  free(display->buffer_empty);
  free(display->buffer_frame);
}

