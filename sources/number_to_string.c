#include "number_to_string.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

char* number_to_string_li(
  long int number
) {
  size_t length = 1;
  static char* result;
  result = malloc(sizeof(char) * length);
  result[0] = '\0';

  if (number < 0) {
    length = length + 1;
    result = realloc(
      result,
      sizeof(char) * length
    );

    result[length - 1] = result[length - 2];
    result[0] = '-';

    number = -number;
  }

  size_t digits_length = 0;
  char* digits;
  digits = malloc(
    sizeof(char) * digits_length
  );

  do {
    long int digit = number;

    if (number > 9) {
      number = number / 10;
      digit = digit - (number * 10);
    } else {
      number = 0;
    }

    digits_length = digits_length + 1;
    digits = realloc(
      digits,
      sizeof(char) * digits_length
    );
    digits[digits_length - 1] = digit + '0';
  } while (number != 0);

  for (
    signed int d = digits_length - 1;
    d >= 0;
    --d
  ) {
    length = length + 1;
    result = realloc(
      result,
      sizeof(char) * length
    );
    result[length - 1] = result[length - 2];
    result[length - 2] = digits[d];
  }

  free(digits);

  return result;
}

char* number_to_string_ui(
  unsigned int number
) {
  size_t length = 1;
  char* result;
  result = malloc(sizeof(char) * length);
  result[0] = '\0';
 
  size_t digits_length = 0;
  char* digits;
  digits = malloc(
    sizeof(char) * digits_length
  );

  do {
    unsigned int digit = number;

    if (number > 9) {
      number = number / 10;
      digit = digit - (number * 10);
    } else {
      number = 0;
    }

    digits_length = digits_length + 1;
    digits = realloc(
      digits,
      sizeof(char) * digits_length
    );
    digits[digits_length - 1] = digit + '0';
  } while (number != 0);

  for (
    signed int d = digits_length - 1;
    d >= 0;
    --d
  ) {
    length = length + 1;
    result = realloc(
      result,
      sizeof(char) * length
    );
    result[length - 1] = result[length - 2];
    result[length - 2] = digits[d];
  }

  free(digits); 
  
  return result;
}

char* number_to_string_mt(
  micro_time number
) {
  size_t length = 1;
  char* result;
  result = malloc(sizeof(char) * length);
  result[0] = '\0';
  
  size_t digits_length = 0;
  char* digits;
  digits = malloc(
    sizeof(char) * digits_length
  );

  do {
    micro_time digit = number;

    if (number > 9) {
      number = number / 10;
      digit = digit - (number * 10);
    } else {
      number = 0;
    }

    digits_length = digits_length + 1;
    digits = realloc(
      digits,
      sizeof(char) * digits_length
    );
    digits[digits_length - 1] = digit + '0';
  } while (number != 0);

  for (
    signed int d = digits_length - 1;
    d >= 0;
    --d
  ) {
    length = length + 1;
    result = realloc(
      result,
      sizeof(char) * length
    );
    result[length - 1] = result[length - 2];
    result[length - 2] = digits[d];
  }

  free(digits);

  return result;
}

