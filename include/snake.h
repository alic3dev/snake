#ifndef __SNAKE_H
#define __SNAKE_H

#include "position.h"

int main();
void interupt_handler(int);
void* get_user_input();
struct position* place_apple(
  struct position**,
  size_t,
  struct position*
);
void initialize_buffer(
  unsigned char*, 
  struct position*
);

#endif

