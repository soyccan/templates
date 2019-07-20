void __merge(int arr[], int begin, int mid, int end) {
  int buf[end+1];

  // left and right pointer
  int l = begin;
  int r = mid + 1;

  for (int i=begin; i<=end; i++) {
    // in one of the following case, push the left element
    // at the back of the buffer, for the right otherwise
    // 1. left pointer is in range but the right one isn't
    // 2. both pointers are in range and the left one's value is smaller  
    if (l <= mid && (r > end || arr[l] <= arr[r]))
      buf[i] = arr[l++];
    else
      buf[i] = arr[r++];
  }

  for (int i=begin; i<=end; i++)
    arr[i] = buf[i];
}

void __split(int arr[], int begin, int end) {
  if (begin < end) {
    int mid = (begin + end) / 2;
    __split(arr, begin, mid);
    __split(arr, mid+1, end);
    __merge(arr, begin, mid, end);
  }
}

void merge_sort(int arr[], int len) {
  __split(arr, 0, len-1);
}