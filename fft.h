// implemented with complex floating number and recursive
// very inefficient

#ifndef _FFT_H
#define _FFT_H

#include <complex>
using std::complex;

#include <vector>
using std::vector;

#include <stdexcept>
using std::invalid_argument;

#include <algorithm>
using std::reverse;

template<typename T>
struct FFT {
    inline static complex<T> omega(int n, int k) {
        // ω = e ^ 2πki/n
        return exp(complex<T>(0, 2.*acos(-1.)*k/n));
    }
    static vector<complex<T>>
    fast_fourier_transform(const vector<complex<T>>& polynomial) {
        size_t n = polynomial.size();
        if ((n & (n-1)) != 0)
            throw invalid_argument("polynomial's length must be power of 2");
        if (n == 1)
            return vector<complex<T>> { polynomial[0] };

        vector<complex<T>> even, odd;
        for (size_t i=0; i<n; i++) {
            if (i % 2 == 0)
                even.push_back(polynomial[i]);
            else
                odd.push_back(polynomial[i]);
        }

        vector<complex<T>> even_val(fast_fourier_transform(even));
        vector<complex<T>> odd_val(fast_fourier_transform(odd));
        vector<complex<T>> val(n);

        // calc val[k] = f(ω_n^k)
        for (size_t k=0; k<n; k++)
            val[k] = even_val[k%(n/2)] + omega(n, k) * odd_val[k%(n/2)];
        return val;
    }
    static vector<complex<T>>
    inverse_fast_fourier_transform(const vector<complex<T>>& polynomial) {
        vector<complex<T>> val(fast_fourier_transform(polynomial));
        reverse(val.begin()+1, val.end());
        for (size_t i=0; i<polynomial.size(); i++)
            val[i] /= (T)polynomial.size();
        return val;
    }
};

#endif
