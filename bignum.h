// TODO: multiplication and division
#ifndef _BIGNUM_H
#define _BIGNUM_H

#define FOR(i,l,r) for (std::size_t i=l; i<r; i++)

#include <vector>
#include <cstddef>
#include <cstdint>
#include <cctype>
#include <string>
#include <istream>
#include <ostream>
#include <cmath>

class BigNum {
public:
    BigNum() {
        _sign = false;
    }
    ~BigNum() {
        _arr.clear();
    }
    BigNum(const BigNum& x) {
        *this = x;
    }
    BigNum(const std::string& x) {
        *this = x;
    }
    BigNum(std::int64_t x) {
        *this = x;
    }
    BigNum& operator=(const BigNum& x) {
        _arr = x._arr;
        _sign = x._sign;
        return *this;
    }
    BigNum& operator=(const std::string& x) {
        // TODO: verify
        _arr.clear();
        _sign = (x[0] == '-');
        for (int i=x.size()-1; i>=0; i--)
            if (std::isdigit(x[i]))
                _arr.push_back(x[i]-'0');
        return *this;
    }
    BigNum& operator=(std::int64_t x) {
        _sign = (x < 0);
        _arr.clear();
        if (x < 0) x = -x;
        while (x > 0) {
            _arr.push_back(x % 10);
            x /= 10;
        }
        return *this;
    }
    BigNum& operator+=(const BigNum& x) {
        if (_sign == x._sign)
            return _iadd(x);
        else
            return _isub(x);
    }
    BigNum& operator-=(const BigNum& x) {
        if (_sign == x._sign)
            return _isub(x);
        else
            return _iadd(x);
    }
    BigNum& operator*=(const BigNum& x) {
        return *this = _mul(*this, x);
    }
    BigNum& operator/=(const BigNum& x) {
        // TODO: O(NlogN)
        return *this;
    }
    friend BigNum operator-(BigNum x) {
        x._sign = !x._sign;
        return x;
    }
    friend BigNum operator+(BigNum x, const BigNum& y) {
        return x += y;
    }
    friend BigNum operator-(BigNum x, const BigNum& y) {
        return x -= y;
    }
    friend BigNum operator*(BigNum x, const BigNum& y) {
        return x *= y;
    }
    friend BigNum operator/(BigNum x, const BigNum& y) {
        return x /= y;
    }
    friend std::ostream& operator<<(std::ostream& os, const BigNum& x) {
        return os << to_string(x);
    }
    friend std::istream& operator>>(std::istream& is, BigNum& x) {
        std::string s;
        is >> s;
        x = s;
        return is;
    }
    friend std::string to_string(const BigNum& x) {
        std::string s;
        if (x._sign) s += '-';
        for (int i=x._arr.size()-1; i>=0; i--)
            s += '0' + x._arr[i];
        return s;
    }
    friend operator==(const BigNum& x, const BigNum& y) {
        if (x._arr.size() != y._arr.size())
            return false;
        for (int i=x._arr.size()-1 ; i>=0; i--)
            if (x._arr[i] != y._arr[i])
                return false;
        return true;
    }
    friend operator!=(const BigNum& x, const BigNum& y) {
        return !(x == y);
    }
    friend operator<(const BigNum& x, const BigNum& y) {
        if (x._arr.size() != y._arr.size())
            return x._arr.size() < y._arr.size();
        for (int i=x._arr.size()-1 ; i>=0; i--)
            if (x._arr[i] != y._arr[i])
                return x._arr[i] < y._arr[i];
        return false; // equal
    }
    friend operator>(const BigNum& x, const BigNum& y) {
        return !(x < y || x == y);
    }
    friend operator<=(const BigNum& x, const BigNum& y) {
        return x < y || x == y;
    }
    friend operator>=(const BigNum& x, const BigNum& y) {
        return !(x < y);
    }
    // std::size_t size() const {
    //     return _arr.size();
    // }

private:
    std::vector<std::int8_t> _arr; // TODO: determine suitable data type
                                   // (not overflowing on multiplication)
    bool _sign; // true for negative

    void _carry() {
        FOR(i, 0, _arr.size()-1) {
            if (_arr[i] >= 0) {
                _arr[i+1] += _arr[i] / 10;
                _arr[i] %= 10;
            } else {
                // warning: floating point unprecision
                int borrow = std::ceil(_arr[i]/-10.);
                _arr[i+1] -= borrow;
                _arr[i] += borrow*10;
             }
        }
        if (_arr.back() < 0) {
            _sign = !_sign;
            FOR(i, 0, _arr.size())
                _arr[i] = -_arr[i];
            _carry();
        }
        while (_arr.size() > 1 && _arr.back() == 0)
            _arr.pop_back();
    }
    BigNum& _iadd(const BigNum& x) { // in place
        _arr.resize(std::max(_arr.size(), x._arr.size()) + 1);
        FOR(i, 0, x._arr.size())
            _arr[i] += x._arr[i];
        _carry();
        return *this;
    }
    BigNum& _isub(const BigNum& x) { // in place
        _arr.resize(std::max(_arr.size(), x._arr.size()) + 1);
        FOR(i, 0, x._arr.size())
            _arr[i] -= x._arr[i];
        _carry();
        return *this;
    }
    friend BigNum _mul(const BigNum& x, const BigNum& y) {
        // TODO: O(N^2) -> FFT -> O(NlogN)
        BigNum z(x._arr.size() + y._arr.size() + 1);
        z._sign = (x._sign != y._sign);
        FOR(i, 0, x._arr.size()) {
            FOR(j, 0, y._arr.size())
                z._arr[i+j] += x._arr[i] * y._arr[j];
            z._carry();
        }
        return z;
    }
};

#undef FOR
#endif
