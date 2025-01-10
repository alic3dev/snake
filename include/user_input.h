#ifndef __USER_INPUT_H
#define __USER_INPUT_H

#include <pthread.h>

#define __USER_INPUT_VALID_INPUTS_LENGTH 14

extern int user_input;
extern int valid_inputs[];

extern unsigned char user_input_thread_running;

extern pthread_mutex_t user_input_thread_running_mutex;
extern pthread_mutex_t user_input_mutex;

extern pthread_t user_input_thread;

void user_input_thread_start();
void user_input_thread_join();

void user_input_destroy();

void* __user_input_get();

#endif

