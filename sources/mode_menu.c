#include "mode_menu.h"
#include <stdio.h>
#include <pthread.h>

#include "display.h"
#include "key_values.h"
#include "mode.h"
#include "user_input.h"

const struct menu_option menu_options[] = {{
  text: "START",
  text_length: 5,
  action: ACTION_START
}, {
  text: "EXIT",
  text_length: 4,
  action: ACTION_EXIT
}};
const size_t menu_options_length = 2;

void mode_menu_initialize(
  struct mode_menu* mode_menu,
  struct display* display
) {
  mode_menu->display = display;

  mode_menu->menu_option_index = 0;

  size_t maximum_option_length = 0;

  for (
    size_t i = 0;
    i < menu_options_length;
    ++i
  ) {
    if (
      menu_options[i].text_length >
      maximum_option_length
    ) {
      maximum_option_length = (
        menu_options[i].text_length
      );
    }
  }

  mode_menu->offset.x = ((
    (mode_menu->display->terminal_size->x - 2) -
    (maximum_option_length + 2)) / 2
  );
  mode_menu->offset.y = ((
    mode_menu->display->terminal_size->y -
    menu_options_length) / 2
  );
}

enum MODE mode_menu_poll(
  struct mode_menu* mode_menu,
  micro_time current_micro_time
) {
  enum menu_action menu_action_selected = (
    ACTION_NONE
  );

  pthread_mutex_lock(&user_input_mutex);
  switch (user_input) {
    case KEY_VALUE_ARROW_UP:
    case KEY_VALUE_W:
    case KEY_VALUE_I:
      if (mode_menu->menu_option_index > 0) {
        mode_menu->menu_option_index -= 1;
        mode_menu->display->should_render = 1;
      }
      break;
    case KEY_VALUE_ARROW_DOWN:
    case KEY_VALUE_S:
    case KEY_VALUE_K:
      if (
        mode_menu->menu_option_index <
        menu_options_length - 1
      ) {
        mode_menu->menu_option_index += 1;
        mode_menu->display->should_render = 1;
      }
      break;
    case KEY_VALUE_ENTER:
    case KEY_VALUE_SPACE:
      menu_action_selected = menu_options[
        mode_menu->menu_option_index
      ].action;
      mode_menu->display->should_render = 1;
      break;
    default:
      break;
  }
  user_input = -1;
  pthread_mutex_unlock(&user_input_mutex);
  
  switch (menu_action_selected) {
    case ACTION_START:
      return MODE_GAME;
    case ACTION_EXIT:
      return MODE_EXIT;
    default:
      break;
  }

  return MODE_MENU;
}

void mode_menu_display(
  struct mode_menu* mode_menu
) {
  if (mode_menu->display->should_render == 1) {
    display_use_frame(mode_menu->display);

    for (
      size_t i = 0;
      i < menu_options_length;
      ++i
    ) {
      size_t index = (
        ((mode_menu->display->terminal_size->x + 
        1) *
        (i + mode_menu->offset.y)) +
        mode_menu->offset.x
      );

      if (mode_menu->menu_option_index == i) {
        mode_menu->display->buffer[index] = '>';
      }

      for (
        size_t c = 0;
        c < menu_options[i].text_length;
        ++c
      ) {
        mode_menu->display->buffer[
          index + 2 + c
        ] = menu_options[i].text[c];
      }
    }
  }
}

void mode_menu_destroy(
  struct mode_menu* mode_menu
) {
}

