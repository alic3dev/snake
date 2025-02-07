#include "user_input.h"

#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include "keep_running.h"
#include "key_values.h"

int user_input = -1;

int valid_inputs[
  __USER_INPUT_VALID_INPUTS_LENGTH
] = {
  KEY_VALUE_ENTER,
  KEY_VALUE_SPACE,
  KEY_VALUE_ARROW_UP,
  KEY_VALUE_ARROW_DOWN,
  KEY_VALUE_ARROW_RIGHT,
  KEY_VALUE_ARROW_LEFT,
  KEY_VALUE_W,
  KEY_VALUE_S,
  KEY_VALUE_D,
  KEY_VALUE_A,
  KEY_VALUE_I,
  KEY_VALUE_K,
  KEY_VALUE_L,
  KEY_VALUE_J
};

unsigned char user_input_thread_running = 0;

pthread_mutex_t user_input_mutex;
pthread_mutex_t user_input_thread_running_mutex;

pthread_t user_input_thread;

void user_input_thread_start() {
  pthread_mutex_init(&user_input_mutex, NULL);
  pthread_mutex_init(
    &user_input_thread_running_mutex,
    NULL
  );

  pthread_create(
    &user_input_thread,
    NULL,
    __user_input_get,
    NULL
  );

  pthread_mutex_lock(
    &user_input_thread_running_mutex
  );
  user_input_thread_running = 1;
  pthread_mutex_unlock(
    &user_input_thread_running_mutex
  );
}

void user_input_thread_join() {
  pthread_join(user_input_thread, NULL);
}

void user_input_destroy() {
  pthread_mutex_destroy(&user_input_mutex);
  pthread_mutex_destroy(
    &user_input_thread_running_mutex
  );
}

void* __user_input_get() { 
  struct termios termios_attrs_original;
  struct termios termios_attrs_updated;
  
  tcgetattr(
    STDIN_FILENO,
    &termios_attrs_original
  );

  tcgetattr(
    STDIN_FILENO,
    &termios_attrs_updated
  );

  termios_attrs_updated.c_lflag &= ~(
    ICANON | ECHO
  );
  
  tcsetattr(
    STDIN_FILENO,
    TCSANOW,
    &termios_attrs_updated
  );

  int _user_input;

  do {
    pthread_mutex_unlock(&keep_running_mutex);

    _user_input = getchar();

    for (
      size_t i = 0;
      i < __USER_INPUT_VALID_INPUTS_LENGTH; 
      i++
    ) {
      if (_user_input == valid_inputs[i]) {
        pthread_mutex_lock(&user_input_mutex);
        user_input = _user_input;
        pthread_mutex_unlock(&user_input_mutex);
        break;
      }
    }

    pthread_mutex_lock(&keep_running_mutex);
  } while (keep_running == 1);
 
  tcsetattr(
    STDIN_FILENO,
    TCSANOW,
    &termios_attrs_original
  );

  pthread_mutex_lock(
    &user_input_thread_running_mutex
  );
  user_input_thread_running = 0;
  pthread_mutex_unlock(
    &user_input_thread_running_mutex
  );
  
  return NULL;
}

