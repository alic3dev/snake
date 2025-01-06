#ifndef __MODE_FUNCTIONS_H
#define __MODE_FUNCTIONS_H

#include "display.h"
#include "micro_time.h" 
#include "mode.h"

void mode_initialize(
  enum MODE,
  void*,
  struct display*
);
enum MODE mode_poll(enum MODE, void*, micro_time);
void mode_display(
  enum MODE,
  void*
);
void mode_destroy(enum MODE, void*);

#endif

