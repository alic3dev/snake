#include "mode_exit.h"

#include <pthread.h>

#include "display.h"
#include "keep_running.h"
#include "micro_time.h"
#include "mode.h"

const unsigned char mode_exit_text[] = (
  "Press any key to exit"
);

const size_t mode_exit_text_length = 21;
const size_t mode_exit_dot_count_max = 3;

void mode_exit_initialize(
  struct mode_exit* mode_exit,
  struct display* display
) {
  mode_exit->display = display;

  mode_exit->offset.x = (
    mode_exit->display->terminal_size->x -
    (mode_exit_text_length +
    mode_exit_dot_count_max)
  ) / 2;
  mode_exit->offset.y = (
    mode_exit->display->terminal_size->y / 2
  ) - 1;

  mode_exit->dot_count = 0;
  mode_exit->micro_time_dot_previous = 0;
  mode_exit->micro_time_dot_speed = 500000;

  mode_exit->started_exiting = 0;
}

enum MODE mode_exit_poll(
  struct mode_exit* mode_exit,
  micro_time micro_time_current
) {
  micro_time micro_time_dot_delta = (
    micro_time_current -
    mode_exit->micro_time_dot_previous
  );

  if (
    micro_time_dot_delta >= 
    mode_exit->micro_time_dot_speed
  ) {
    if (
      mode_exit->dot_count >= 
      mode_exit_dot_count_max
    ) {
      mode_exit->dot_count = 0;
    } else {
      mode_exit->dot_count += 1;
    }

    mode_exit->micro_time_dot_previous = (
      micro_time_current
    );
  }

  if (mode_exit->started_exiting == 1) {
    return MODE_EXIT;
  }

  pthread_mutex_lock(&keep_running_mutex);
  keep_running = 0;
  pthread_mutex_unlock(&keep_running_mutex);

  mode_exit->started_exiting = 1;

  return MODE_EXIT;
}

void mode_exit_display(
  struct mode_exit* mode_exit
) {
  if (mode_exit->display->should_render != 1) {
    return;
  }

  display_use_frame(mode_exit->display);

  for (
    size_t i = 0;
    i < mode_exit_text_length;
    ++i
  ) {
    mode_exit->display->buffer[
      mode_exit->offset.x +
      (mode_exit->offset.y * (
        mode_exit->display->terminal_size->x + 1
      )) + i
    ] = mode_exit_text[i];
  }

  for (
    size_t i = 0;
    i < mode_exit->dot_count;
    ++i
  ) {
    mode_exit->display->buffer[
      mode_exit->offset.x +
      (mode_exit->offset.y * (
        mode_exit->display->terminal_size->x + 1
      )) + mode_exit_text_length + i
    ] = '.';
  }
}

void mode_exit_destroy(
  struct mode_exit* mode_exit
) {
}

