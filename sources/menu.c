#include "menu.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "display.h"
#include "key_values.h"
#include "position.h"
#include "user_input.h"

void menu_initialize(
  struct menu* menu,
  struct display* display
) {
  menu->options_length = 0;
  menu->options = malloc(
    sizeof(struct menu_option) *
    menu->options_length
  );
  menu->index = 0;
  menu->maximum_label_length = 0;
  menu->display = display;
  menu->offset.x = 0;
  menu->offset.y = 0;
}

void menu_add_option(
  struct menu* menu,
  char* label,
  enum menu_action action
) {
  menu->options_length = menu->options_length + 1;
  menu->options = realloc(
    menu->options,
    sizeof(
      struct menu_option
    ) * menu->options_length
  );

  size_t option_index = menu->options_length - 1;

  menu->options[
    option_index
  ].label_length = strlen(label);

  menu->options[
    option_index
  ].label = malloc(
    sizeof(char) * (
      menu->options[option_index].label_length
    )
  );

  strcpy(
    menu->options[option_index].label,
    label
  );

  menu->options[
    option_index
  ].action = action;

  if (
    menu->options[option_index].label_length >
    menu->maximum_label_length
  ) {
    menu->maximum_label_length = (
      menu->options[option_index].label_length
    );
  }
}

void menu_offset_center(struct menu* menu) {
  menu->offset.x = ((
    (menu->display->terminal_size->x - 2) -
    (menu->maximum_label_length + 2))
    / 2
  );
  menu->offset.y = ((
    menu->display->terminal_size->y -
    menu->options_length) / 2
  );
}

enum menu_action menu_poll(struct menu* menu) {
  enum menu_action menu_action_selected = (
    MENU_ACTION_NONE
  );

  pthread_mutex_lock(&user_input_mutex);
  switch (user_input) {
    case KEY_VALUE_ARROW_UP:
    case KEY_VALUE_W:
    case KEY_VALUE_I:
      if (menu->index > 0) {
        menu->index -= 1;
        menu->display->should_render = 1;
      }
      break;
    case KEY_VALUE_ARROW_DOWN:
    case KEY_VALUE_S:
    case KEY_VALUE_K:
      if (
        menu->index <
        menu->options_length - 1
      ) {
        menu->index += 1;
        menu->display->should_render = 1;
      }
      break;
    case KEY_VALUE_ENTER:
    case KEY_VALUE_SPACE:
      menu_action_selected = menu->options[
        menu->index
      ].action;
      menu->display->should_render = 1;
      break;
    default:
      break;
  }
  user_input = -1;
  pthread_mutex_unlock(&user_input_mutex);

  return menu_action_selected;
}

void menu_display(struct menu* menu) {
  if (menu->display->should_render == 1) {
    for (
      size_t i = 0;
      i < menu->options_length;
      ++i
    ) {
      size_t index = (
        ((menu->display->terminal_size->x +
          1) *
        (i + menu->offset.y)) +
        menu->offset.x
      );

      if (menu->index == i) {
        menu->display->buffer[index] = '>';
      }

      for (
        size_t c = 0;
        c < menu->options[i].label_length;
        ++c
      ) {
        menu->display->buffer[
          index + 2 + c
        ] = menu->options[i].label[c];
      }
    }
  }
}

void menu_destroy(struct menu* menu) {
  for (
    size_t o = 0; o < menu->options_length; o++
  ) {
    free(menu->options[o].label);
  }

  free(menu->options);
}

