#include <signal.h>
#include <stddef.h>

#include "interupt_handler.h"

unsigned char interupted = 0;

void interupt_handler_initialize() {
  struct sigaction signal_action;
  signal_action.sa_handler = interupt_handler;
  sigaction(SIGINT, &signal_action, NULL);
}

void interupt_handler(int _) {
  interupted = 1;
}

