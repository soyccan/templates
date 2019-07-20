// TODO: type generic

#include <stdlib.h>
#include <math.h>

int pow2(int exponent) {
  return 1 << exponent;
}

void sparse_table(int** table, int* arr, int len) {
  for (int i=0; i<len; i++) {
    table[0][i] = arr[i];
  }
  for (int i=1; i<=log2(len); i++) {
    for (int j=0; j<=len-pow2(i); j++) {
      table[i][j] = fmin(table[i-1][j], table[i-1][j+pow2(i-1)]);
    }
  }
}

int query(int** table, int begin, int end) {
  int l = log2(abs(begin-end)+1);
  return fmin(table[l][begin], table[l][end-pow2(l)+1]);
}