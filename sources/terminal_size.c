#include <sys/ioctl.h>
#include <stddef.h>
#include <stdlib.h>

#include "position.h"

struct position* get_terminal_size() {
  struct winsize _terminal_size;

  if (
    ioctl(0, TIOCGWINSZ, &_terminal_size) == -1
  ) {
    return NULL;
  }

  static struct position* terminal_size; 
  terminal_size = malloc(sizeof(struct position));
  terminal_size->x = _terminal_size.ws_col;
  terminal_size->y = _terminal_size.ws_row;

  return terminal_size;
}

