#include <vector>

template<typename OutputIterator>
OutputIterator get_primes(int start, int end, OutputIterator result) {
  std::vector<bool> primes(end+1, true);
  for (int i=max(2, start); i<=end; i++) {
    if (primes[i]) {
      *result++ = i;
      for (int j=i+i; j<=end; j+=i)
        primes[j] = false;
    }
  }
  return result;
}