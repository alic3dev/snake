#include "mode_intro.h"

#include "display.h"
#include "micro_time.h"
#include "mode.h"

void mode_intro_initialize(
  struct mode_intro* mode_intro
) {
}

enum MODE mode_intro_poll(
  struct mode_intro* mode_intro,
  micro_time current_micro_time
) {
  return INTRO;
}

void mode_intro_display(
  struct mode_intro* mode_intro,
  struct display* display
) {
}

void mode_intro_destroy(
  struct mode_intro* mode_intro
) {
}

