#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake.h"
#include "display.h"
#include "keep_running.h"
#include "micro_time.h"
#include "mode_all.h"
#include "interupt_handler.h"
#include "position.h"
#include "terminal_size.h"
#include "user_input.h"

int main() {
  srand(get_micro_time());

  keep_running_initialize();
  interupt_handler_initialize();

  struct position terminal_size;
  terminal_size.x = 0;
  terminal_size.y = 0;
  terminal_size_initialize(&terminal_size);

  if (
    terminal_size.x == 0 || terminal_size.y == 0
  ) {
    fprintf(
      stderr,
      "Couldn't determine terminal size\n"
    );

    return 1;
  }

  struct display display;
  display_initialize(&display, &terminal_size);
  
  struct mode_game mode_game;
  mode_initialize(GAME, &mode_game, &display);
  
  enum MODE mode_current = GAME;
  void* mode_struct = &mode_game;

  user_input_thread_start();

  unsigned char _keep_running = 1;
  while (_keep_running) {
    micro_time time_current = (
      get_micro_time()
    );

    mode_poll(
      mode_current,
      mode_struct,
      time_current
    );
    mode_display(mode_current, mode_struct);

    display_print(&display);

    if (interupted == 1) {
      _keep_running = 0;
      pthread_mutex_lock(&keep_running_mutex);
      keep_running = 0;
      pthread_mutex_unlock(&keep_running_mutex);
    }
  }

  printf("Press any key to exit\n");
  user_input_thread_join();
  user_input_destroy();

  keep_running_destroy();
  
  mode_destroy(GAME, &mode_game);
  display_destroy(&display);

  return 0;
}

