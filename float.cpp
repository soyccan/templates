#include <cmath>

template<typename T>
class _Float {
private:
    static constexpr T EPS = 1e-10;
    T __num;
public:
    _Float(T x) { __num = x; }
    _Float& operator=(T x) { __num = x; return *this; }

    template<typename _T>
    friend bool operator==(const _Float<_T>&, const _Float<_T>&);
    template<typename _T>
    friend bool operator<(const _Float<_T>&, const _Float<_T>&);
};

template<typename T>
bool operator==(const _Float<T>& x, const _Float<T>& y) {
    return std::abs(x.__num - y.__num) < _Float<T>::EPS;
}

template<typename T>
bool operator<(const _Float<T>& x, const _Float<T>& y) {
    return (x - y).__num < -_Float<T>::EPS;
}

template<typename T>
bool operator!=(const _Float<T>& x, const _Float<T>& y) {
    return !(x == y);
}

template<typename T>
bool operator<=(const _Float<T>& x, const _Float<T>& y) {
    return x < y || x == y;
}

template<typename T>
bool operator>=(const _Float<T>& x, const _Float<T>& y) {
    return !(x < y);
}

template<typename T>
bool operator>(const _Float<T>& x, const _Float<T>& y) {
    return !(x < y || x == y);
}

typedef _Float<long double> MyFloat;


// test
#include <iostream>
using namespace std;
int main() {
    MyFloat a=0, b=9;
    cout<<(a>b);
    return 0;
}
