#include "keep_running.h"

#include <pthread.h>
#include <stddef.h>

unsigned char keep_running = 1;
pthread_mutex_t keep_running_mutex;

void keep_running_initialize() {
  pthread_mutex_init(&keep_running_mutex, NULL);
}

void keep_running_destroy() {
  pthread_mutex_destroy(&keep_running_mutex);
}

