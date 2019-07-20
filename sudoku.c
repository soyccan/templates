#include <stdarg.h>
#include <stdio.h>
#include <string.h>


/**
 * modified 'sprintf' function
 * require 'str' to be initialized
 * append instead of override
 */
int my_sprintf(char *str, const char *format, ...) {
  va_list args;
  va_start(args, format);
  return sprintf(str+strlen(str), format, va_arg(args, void*));
}

int main() {
  char result[300] = "";

  for (int i=0; i<9; i++) {
    char input[10] = "";

    printf("? ");
    scanf("%s", input);

    for (int j=0; j<9; j++) {
      my_sprintf(result, "%c ", input[j]);
      if (j == 2 || j == 5) {
        my_sprintf(result, "| ");
      }
    }

    my_sprintf(result, "\n");
    if (i == 2 || i == 5) {
      my_sprintf(result, "----------------------\n");
    }
  }

  printf("%s", result);

  return 0;
}