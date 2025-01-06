#include <sys/ioctl.h>
#include <stddef.h>

#include "position.h"

void terminal_size_initialize(
  struct position* terminal_size
) {
  struct winsize _terminal_size;

  if (
    ioctl(0, TIOCGWINSZ, &_terminal_size) == -1
  ) {
    return;
  }

  terminal_size->x = _terminal_size.ws_col;
  terminal_size->y = _terminal_size.ws_row - 1;
}

