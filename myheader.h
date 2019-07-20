#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define max(x, y) \
  (x > y ? x : y)

#define min(x, y) \
  (x < y ? x : y)

#define for_range(var, stop) \
  for (int var=0; var<stop; var++)

#define for_times(times) \
  for (int _=times; _>0; _--)

#define for_each(type, ptr, arr) \
  for (type* ptr=arr; ptr<arr+arrlen(arr); ptr++)

#define for_each_char(ptr, str) \
  for (char* ptr=str; *ptr!='\0'; ptr++)

#define arrlen(arr) \
  (sizeof arr / sizeof *arr)

#define swap(x, y) {\
  int _ = x;\
  x = y;\
  y = _;\
}

// append
#define sprintf_a(buffer, format, ...) \
  sprintf(buffer+strlen(buffer), format, ##__VA_ARGS__)


int** matrix(int row, int col) {
  int** m = calloc(row, sizeof(int*));
  for (int i=0; i<row; i++) {
    m[i] = calloc(col, sizeof(int));
  }
  return m;
}