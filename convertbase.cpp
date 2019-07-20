// TODO: nagetive

#include <algorithm>
using std::reverse;

#include <cmath>
using std::pow;

#include <string>
using std::string;


string convert(string num, int from_base, int to_base) {
  if (from_base > 10 or to_base > 10)
    return "";
  if (num == "0" or from_base == to_base)
    return num;

  // to decimal
  reverse(num.begin(), num.end());
  int decimal = 0;
  for (unsigned i=0; i<num.length(); i++) {
    decimal += (num[i] - '0') * pow(from_base, i);
  }

  // to required base
  string result;
  while (decimal != 0) {
    result += '0' + decimal%to_base;
    decimal /= to_base;
  }
  reverse(result.begin(), result.end());
  return result;
}