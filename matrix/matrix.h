// TODO: 1. solve equation
//          for infinite solutions, use parametric equation
//       2. pass 'rows' and 'cols' as template parameters
//       3. replace dynamic vector with static array
#ifndef _MATRIX_H
#define _MATRIX_H
#define FOR(i,start,end) for (std::size_t i=start; i<end; ++i)

#include <vector>
#include <stdexcept>
#include <ostream>
#include <cstddef>
#include <algorithm>
#include <type_traits>

namespace matrix {
    const long double EPS = 1e-10;

    inline int sgn(long double x) {
        if (x < -EPS) return -1;
        else if (x > EPS) return 1;
        else return 0;
    }

    template<typename T = long double>
    class Matrix {
    public:
        Matrix():
            _arr() {}

        Matrix(std::size_t rows, std::size_t cols):
            _arr(rows, std::vector<T>(cols)) {}

        Matrix(std::initializer_list<std::vector<T>> list):
            _arr(list) {}

        Matrix(std::vector<std::vector<T>> vec):
            _arr(vec) {}

        inline std::size_t rows() const {
            return _arr.size();
        }
        inline std::size_t cols() const {
            if (_arr.size())
                return _arr[0].size();
            else
                return 0;
        }
        bool empty() const {
            return rows() == 0 && cols() == 0;
        }
        const std::vector<T>& operator[](std::size_t index) const {
            return _arr[index];
        }
        std::vector<T>& operator[](std::size_t index) {
            return _arr[index];
        }
        Matrix& operator+=(const Matrix& x) {
            if (rows() != x.rows() || cols() != x.cols())
                throw std::invalid_argument("matrix size not match");
            FOR(i, 0, rows())
                FOR(j, 0, cols())
                    _arr[i][j] += x[i][j];
            return *this;
        }
        Matrix& operator-=(const Matrix& x) {
            return *this += -x;
        }
        Matrix& operator*=(T t) {
            FOR(i, 0, rows())
                FOR(j, 0, cols())
                    _arr[i][j] *= t;
            return *this;
        }
        Matrix& operator/=(T t) {
            FOR(i, 0, rows())
                FOR(j, 0, cols())
                    _arr[i][j] /= t;
            return *this;
        }
        Matrix& operator%=(T t) {
            FOR(i, 0, rows())
                FOR(j, 0, cols())
                    _arr[i][j] %= t;
            return *this;
        }

        friend Matrix operator-(Matrix x) { return x *= -1; }
        friend Matrix operator+(Matrix x, const Matrix& y) { return x += y; }
        friend Matrix operator-(Matrix x, const Matrix& y) { return x -= y; }
        friend Matrix operator*(Matrix x, T y) { return x *= y; }
        friend Matrix operator*(T y, Matrix x) { return x *= y; }
        friend Matrix operator/(Matrix x, T y) { return x /= y; }
        friend Matrix operator%(Matrix x, T y) { return x %= y; }
        friend std::ostream& operator<<(std::ostream& os, const Matrix& x) {
            FOR(i, 0, x.rows()) {
                FOR(j, 0, x.cols()) {
                    os << x._arr[i][j];
                    if (j == x.cols()-1)
                        os << '\n';
                    else
                        os << ',' << ' ';
                }
            }
            return os;
        }
        friend Matrix operator*(const Matrix& x, const Matrix& y) {
            return mul(x, y);
        }
        friend bool operator==(const Matrix& x, const Matrix& y) {
            if (x.rows() != y.rows() || x.cols() != y.cols())
                return false;
            FOR(i, 0, x.rows())
                FOR(j, 0, x.cols())
                    if (sgn(x[i][j] - y[i][j]) != 0)
                        return false;
            return true;
        }
        friend bool operator!=(const Matrix& x, const Matrix& y) {
            return !(x == y);
        }

        bool gauss() {
            // Gaussian elimination
            // transform matrix into row echelon form
            // only use row switching and row addition
            // can be used to calculate determinant
            bool sign = 0; // whether determinant should change sign (after row switching)
            FOR(i, 0, rows()) {
                for (std::size_t j = i+1; j < rows() && sgn(_arr[i][i]) == 0; ++j) {
                    std::swap(_arr[i], _arr[j]);
                    sign = !sign;
                }
                if (sgn(_arr[i][i]) == 0)
                    continue;

                FOR(j, i+1, rows()) {
                    T t = _arr[j][i] / _arr[i][i];
                    FOR(k, 0, cols()) {
                        _arr[j][k] -= _arr[i][k] * t;
                    }
                    _arr[j][i] = 0;
                }
            }
            return sign;
        }

    private:
        std::vector< std::vector<T> > _arr;
    };

    template<typename T = long double>
    class SquareMatrix : public Matrix<T> {
    public:
        SquareMatrix(std::size_t size = 0):
            Matrix<T>(size, size) {}

        SquareMatrix(std::initializer_list<std::vector<T>> list):
            Matrix<T>(list) {}

        SquareMatrix(std::vector<std::vector<T>> vec):
            Matrix<T>(vec) {}

        SquareMatrix(const Matrix<T>& matrix):
            Matrix<T>(matrix) {}

        template<typename Int>
        friend SquareMatrix operator^(const SquareMatrix& x, Int y) {
            return pow(x, y);
        }
    };



    template<typename T>
    Matrix<T> augment(const Matrix<T>& x, const Matrix<T>& y) {
        if (x.rows() != y.rows())
            throw std::invalid_argument("different row count");
        Matrix<T> z(x.rows(), x.cols() + y.cols());
        FOR(i, 0, x.rows()) {
            FOR(j, 0, x.cols())
                z[i][j] = x[i][j];
            FOR(j, 0, y.cols())
                z[i][ j+x.cols() ] = y[i][j];
        }
        return z;
    }

    template<typename T>
    Matrix<T> mul(const Matrix<T>& x, const Matrix<T>& y) {
        if (x.cols() != y.rows())
            throw std::invalid_argument("matrix size not match");
        Matrix<T> z(x.rows(), y.cols());
        FOR(i, 0, x.rows())
            FOR(j, 0, y.cols())
                FOR(k, 0, x.cols())
                    z[i][j] += x[i][k] * y[k][j];
        return z;
    }

    template<typename Int>
    Matrix<Int> mul(const Matrix<Int>& x, const Matrix<Int>& y, Int mod) {
        // modulo acts only on integral types
        if (x.cols() != y.rows())
            throw std::invalid_argument("matrix size not match");
        Matrix<Int> z(x.rows(), y.cols());
        FOR(i, 0, x.rows())
            FOR(j, 0, y.cols())
                FOR(k, 0, x.cols()) {
                    z[i][j] += x[i][k] * y[k][j];
                    z[i][j] %= mod;
                }
        return z;
    }

    template<typename T>
    SquareMatrix<T> identity_matrix(std::size_t size) {
        SquareMatrix<T> x(size);
        FOR(i, 0, size) x[i][i]=1;
        return x;
    }

    template<typename T, typename Int>
    SquareMatrix<T> pow(const SquareMatrix<T>& matrix, Int exp) {
        // quick exponentiation
        Matrix<T> x = matrix;
        Matrix<T> res = identity_matrix<T>(matrix.rows());
        Int e = abs(exp);
        while (e > 0) {
            if (e % 2 == 1)
                res = mul(x, res);
            x = mul(x, x);
            e /= 2;
        }

        if (exp >= 0)
            return SquareMatrix<T>(res);
        else
            return inverse(SquareMatrix<T>(res));
    }

    template<typename Int>
    SquareMatrix<Int> pow(const SquareMatrix<Int>& matrix, Int exp, Int mod) {
        // quick exponentiation
        Matrix<Int> x = matrix;
        Matrix<Int> res = identity_matrix<Int>(matrix.rows());
        Int e = abs(exp);
        while (e > 0) {
            if (e % 2 == 1)
                res = mul(x, res, mod);
            x = mul(x, x, mod);
            e /= 2;
        }

        if (exp >= 0)
            return SquareMatrix<Int>(res);
        else
            return inverse(SquareMatrix<Int>(res));
    }

    template<typename T>
    Matrix<T> transpose(const Matrix<T>& x) {
        Matrix<T> y(x.cols(), x.rows());
        FOR(i, 0, y.rows())
            FOR(j, 0, y.cols())
                y[i][j] = x[j][i];
        return y;
    }

    template<typename T>
    SquareMatrix<T> upper_triangular(const SquareMatrix<T>& matrix) {
        SquareMatrix<T> res = matrix;
        res.gauss();
        return res;
    }

    template<typename T>
    T determinant(const SquareMatrix<T>& x) {
        SquareMatrix<T> y = x;
        int sign = y.gauss();
        T res = 1;
        FOR(i, 0, y.rows()) res *= y[i][i];
        if (sign == 1) return -res;
        else if (sign == 0) return res;
        else return 0;
    }

    template<typename T>
    SquareMatrix<T> inverse(const SquareMatrix<T>& matrix) {
        if (sgn(determinant(matrix)) == 0)
            throw std::runtime_error("matrix not invertible");

        SquareMatrix<T> I = identity_matrix<T>(matrix.rows());
        Matrix<T> aug = augment(matrix, I);

        aug.gauss();
        FOR(i, 0, aug.rows()) {
            T lead = aug[i][i];
            if (sgn(lead) != 0)
                FOR(k, i, aug.cols())
                    aug[i][k] /= lead;
            FOR(j, 0, i) {
                T t = aug[j][i];
                FOR(k, i, aug.cols())
                    aug[j][k] -= aug[i][k] * t;
            }
        }

        SquareMatrix<T> res(matrix.rows());
        FOR(i, 0, matrix.rows())
            FOR(j, 0, matrix.cols())
                res[i][j] = aug[i][ j+matrix.cols() ];
        return res;
    }
}

#undef FOR
#endif
