#ifndef __MENU_H
#define __MENU_H

#include <stddef.h>

#include "display.h"
#include "position.h"

enum menu_action {
  MENU_ACTION_START,
  MENU_ACTION_EXIT,
  MENU_ACTION_NONE
};

struct menu_option {
  char* label;
  size_t label_length;
  short int action;
};

struct menu {
  struct menu_option* options;
  size_t options_length;
  
  unsigned short int index; 
  
  size_t maximum_label_length;

  struct display* display;

  struct position offset;
};

void menu_initialize(
  struct menu*,
  struct display*
);

void menu_add_option(
  struct menu*,
  char* label,
  enum menu_action action
);

void menu_offset_center(struct menu*);

enum menu_action menu_poll(struct menu*);

void menu_display(struct menu*);

void menu_destroy(struct menu*);

#endif

