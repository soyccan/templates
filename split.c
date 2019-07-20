#include <stdlib.h>
#include <string.h>


typedef struct {
  char** arr;
  int len;
} strarr_t;

strarr_t split(char* str, const char* delim) {
  strarr_t result;
  result.arr = (char**) malloc(strlen(str) * sizeof(char*));
  result.len = 0;

  for (char* item = strtok(str, delim);
      item != NULL;
      item = strtok(NULL, delim)) {
    result.arr[result.len] = (char*) malloc(strlen(item) * sizeof(char));
    strcpy(result.arr[result.len], item);
    result.len++;
  }
  result.arr = (char**) realloc(result.arr, result.len * sizeof(char*));

  return result;
}