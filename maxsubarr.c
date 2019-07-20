#include <stdio.h>
#define max(x, y) (x > y ? x : y)

long long max_subarr(int arr[], int len) {
  long long partial_sum;
  long long max_sum;
  max_sum = partial_sum = arr[0];

  for (int i=1; i<len; i++) {
    partial_sum = max(arr[i], partial_sum+arr[i]);
    max_sum = max(max_sum, partial_sum);
  }

  return max_sum;
}