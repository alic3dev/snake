#include "mode_gameover.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "display.h"
#include "menu.h"
#include "mode.h"
#include "score.h"

void mode_gameover_initialize(
  struct mode_gameover* mode_gameover,
  struct display* display
) {
  mode_gameover->display = display;
  
  menu_initialize(
    &mode_gameover->menu,
    mode_gameover->display
  );

  size_t score_string_length = 8;
  char* score_string = malloc(
    sizeof(char) * score_string_length
  );
  strcpy(score_string, "SCORE: ");

  long int score_tally = (
    mode_gameover->score->score
  );

  do {
    char score_char = '0' + (score_tally % 10);

    if (score_tally < 10) {
      score_tally = 0;
    } else {
      score_tally = score_tally / 10;
    }

    score_string_length = (
      score_string_length + 1
    );

    score_string = realloc(
      score_string,
      sizeof(char) * score_string_length
    );

    score_string[score_string_length - 2] = (
      score_char
    );
  } while (score_tally != 0);

  for (
    size_t i = 0;
    i < (score_string_length - 8) / 2;
    ++i
  ) {
    score_string[6] = score_string[i + 7];
    score_string[i + 7] = score_string[
      score_string_length - (i + 2)
    ];
    score_string[
      score_string_length - (i + 2)
    ] = (
      score_string[6]
    );
  }

  score_string[6] = ' ';
  
  menu_option_add(
    &mode_gameover->menu,
    score_string,
    MENU_ACTION_NONE
  );

  free(score_string);

  menu_option_add(
    &mode_gameover->menu,
    "NEW GAME",
    MENU_ACTION_START
  );

  menu_option_add(
    &mode_gameover->menu,
    "EXIT",
    MENU_ACTION_EXIT
  );

  menu_index_next(&mode_gameover->menu);

  menu_offset_center(&mode_gameover->menu);
}

enum MODE mode_gameover_poll(
  struct mode_gameover* mode_gameover,
  micro_time current_micro_time
) {
  enum menu_action menu_action_selected = (
    menu_poll(&mode_gameover->menu)
  );

  switch (menu_action_selected) {
    case MENU_ACTION_START:
      return MODE_GAME;
    case MENU_ACTION_EXIT:
      return MODE_EXIT;
    default:
      break;
  }

  return MODE_GAMEOVER;
}

void mode_gameover_display(
  struct mode_gameover* mode_gameover
) {
  if (
    mode_gameover->display->should_render == 1
  ) {
    display_use_frame(mode_gameover->display);

    menu_display(&mode_gameover->menu);
  }
}

void mode_gameover_destroy(
  struct mode_gameover* mode_gameover
) {
  menu_destroy(&mode_gameover->menu);
}

