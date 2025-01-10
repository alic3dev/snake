#ifndef __MODE_MENU_H
#define __MODE_MENU_H

#include "display.h"
#include "micro_time.h"
#include "mode.h"
#include "position.h"

struct mode_menu {
  struct display* display;
  struct position offset;
  size_t menu_option_index;
};

enum menu_action {
  ACTION_EXIT,
  ACTION_NONE,
  ACTION_START
};

struct menu_option {
  char* text;
  size_t text_length;
  enum menu_action action;
};

extern const struct menu_option menu_options[];
extern const size_t menu_options_length;

void mode_menu_initialize(
  struct mode_menu*,
  struct display*
);
enum MODE mode_menu_poll(
  struct mode_menu*,
  micro_time
);
void mode_menu_display(struct mode_menu*);
void mode_menu_destroy(struct mode_menu*);

#endif

