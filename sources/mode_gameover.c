#include "mode_gameover.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "display.h"
#include "menu.h"
#include "mode.h"
#include "number_to_string.h"
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

  char* score_number_string = number_to_string_li(
    mode_gameover->score->score
  );
  size_t score_number_string_length = strlen(
    score_number_string
  );

  score_string = realloc(
    score_string,
    sizeof(char) * (
      score_string_length +
      score_number_string_length
    )
  );

  strcat(score_string, score_number_string);

  free(score_number_string);

  menu_option_add(
    &mode_gameover->menu,
    score_string,
    MENU_ACTION_NONE
  );

  free(score_string);

  size_t apples_eaten_string_length = 9;
  char* apples_eaten_string = malloc(
    sizeof(char) * apples_eaten_string_length
  );
  strcpy(apples_eaten_string, "APPLES: ");

  char* apples_eaten_number_string = (
    number_to_string_ui(
      mode_gameover->score->apples_eaten
    )
  );
  size_t apples_eaten_number_string_length = (
    strlen(apples_eaten_number_string)
  );

  apples_eaten_string = realloc(
    apples_eaten_string,
    sizeof(char) * (
      apples_eaten_string_length +
      apples_eaten_number_string_length
    )
  );

  strcat(
    apples_eaten_string,
    apples_eaten_number_string
  );

  free(apples_eaten_number_string);

  menu_option_add(
    &mode_gameover->menu,
    apples_eaten_string,
    MENU_ACTION_NONE
  );

  free(apples_eaten_string);

  size_t total_time_string_length = 7;
  char* total_time_string = malloc(
    sizeof(char) * total_time_string_length
  );
  strcpy(total_time_string, "TIME: ");

  char* total_time_number_string = (
    number_to_string_li(
      mode_gameover->score->total_time /
      1000000
    )
  );
  size_t total_time_number_string_length = strlen(
    total_time_number_string
  );

  total_time_string = realloc(
    total_time_string,
    sizeof(char) * (
      total_time_string_length +
      total_time_number_string_length
    )
  );

  strcat(
    total_time_string,
    total_time_number_string
  );

  free(total_time_number_string);

  menu_option_add(
    &mode_gameover->menu,
    total_time_string,
    MENU_ACTION_NONE
  );

  free(total_time_string);

  menu_option_add(
    &mode_gameover->menu,
    "",
    MENU_ACTION_NONE
  );

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

