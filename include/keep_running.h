#ifndef __KEEP_RUNNING_H
#define __KEEP_RUNNING_H

#include <pthread.h>

extern unsigned char keep_running;

extern pthread_mutex_t keep_running_mutex;

void keep_running_initialize();
void keep_running_destroy();

#endif

