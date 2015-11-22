#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#define LOG_ERROR(msg, ...) \
  do {\
    fprintf(stderr, "[ERROR] " msg " %s, %d\n", \
      __FILE__, __LINE__, ##__VA_ARGS__);\
  } while (0)

#endif
