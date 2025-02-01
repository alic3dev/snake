#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake.h"
#include "display.h"
#include "interupt_handler.h"
#include "keep_running.h"
#include "micro_time.h"
#include "mode_all.h"
#include "options.h"
#include "position.h"
#include "print_usage.h"
#include "score.h"
#include "terminal_size.h"
#include "user_input.h"

int main(int argc, char** argv) {
  struct options options;
  unsigned char options_invalid = (
    options_initialize(&options, argc, argv)
  );

  if (options_invalid == 1) {
    print_usage(1);
    return options_invalid;
  }

  srand(options.seed);

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

  struct score score;

  struct mode_exit mode_exit;
  struct mode_game mode_game;
  struct mode_gameover mode_gameover;
  struct mode_intro mode_intro;
  struct mode_menu mode_menu;

  mode_game.score = &score;
  mode_gameover.score = &score;

  mode_initialize(
    MODE_EXIT,
    &mode_exit,
    &display
  );
  mode_initialize(
    MODE_GAME,
    &mode_game,
    &display
  );
  mode_initialize(
    MODE_GAMEOVER,
    &mode_gameover,
    &display
  );
  mode_initialize(
    MODE_INTRO,
    &mode_intro,
    &display
  );
  mode_initialize(
    MODE_MENU,
    &mode_menu,
    &display
  );

  enum MODE mode_current;
  void* mode_struct;

  if (options.skip_intro == 1) {
    mode_current = MODE_MENU;
    mode_struct = &mode_menu;
  } else {
    mode_current = MODE_INTRO;
    mode_struct = &mode_intro;
  }

  enum MODE mode_previous = mode_current;
  
  user_input_thread_start();

  unsigned char _keep_running = 1;
  while (_keep_running == 1) {
    if (interupted == 1) {
      mode_current = MODE_EXIT;
      mode_previous = MODE_EXIT;
      mode_struct = &mode_exit;

      display.should_render = 1;
    }

    micro_time time_current = (
      get_micro_time()
    );

    mode_current = mode_poll(
      mode_current,
      mode_struct,
      time_current
    );

    if (mode_current != mode_previous) {
      switch (mode_current) {
        case MODE_EXIT:
          mode_struct = &mode_exit;
          break;
        case MODE_GAME:
          mode_struct = &mode_game;

          mode_destroy(MODE_GAME, &mode_game);

          mode_initialize(
            MODE_GAME,
            &mode_game,
            &display
          );
          break;
        case MODE_GAMEOVER:
          mode_struct = &mode_gameover;

          mode_destroy(
            MODE_GAMEOVER,
            &mode_gameover
          );

          mode_initialize(
            MODE_GAMEOVER,
            &mode_gameover,
            &display
          );
          break;
        case MODE_INTRO:
          mode_struct = &mode_intro;
          break;
        case MODE_MENU:
          mode_struct = &mode_menu;
          break;
        default:
          mode_current = MODE_EXIT;
          mode_struct = &mode_exit;
          break;
      }
      
      display.should_render = 1;

      mode_previous = mode_current;
      continue;
    }

    mode_display(mode_current, mode_struct);

    display_print(&display);

    if (mode_current == MODE_EXIT) {
      pthread_mutex_lock(
        &user_input_thread_running_mutex
      );
      if (user_input_thread_running == 0) {
        _keep_running = 0;
      }
      pthread_mutex_unlock(
        &user_input_thread_running_mutex
      );
    }
  }

  user_input_thread_join();
  user_input_destroy();

  keep_running_destroy();
  
  mode_destroy(MODE_EXIT, &mode_exit);
  mode_destroy(MODE_GAME, &mode_game);
  mode_destroy(MODE_GAMEOVER, &mode_gameover);
  mode_destroy(MODE_INTRO, &mode_intro);
  mode_destroy(MODE_MENU, &mode_menu);

  display_destroy(&display);

  return 0;
}

