int lower_bound(int arr[], int len, int value) {
  int l = 0;
  int r = len - 1;
  while (l < r) {
    int mid = (l + r) / 2;
    if (value > arr[mid])
      l = mid + 1;
    else
      r = mid;
  }
  return l;
}

int upper_bound(int arr[], int len, int value) {
  int l = 0;
  int r = len - 1;
  while (l < r) {
    int mid = (l + r) / 2;
    if (value >= arr[mid]) // the only difference is equal sign
      l = mid + 1;
    else
      r = mid;
  }
  return l;
}