#include "MatrixFibCounter.h"
#include <string>
#include <cstdint>

/* ¬водим структуру матрицы 2*2 */
struct Matrix2x2 {
    uint64_t a, b, c, d;
    Matrix2x2(uint64_t a_, uint64_t b_, uint64_t c_, uint64_t d_)
        : a(a_), b(b_), c(c_), d(d_) {
    }

    // определ€ем умножение матриц
    Matrix2x2 operator*(const Matrix2x2& other) const {
        return {
            a * other.a + b * other.c,
            a * other.b + b * other.d,
            c * other.a + d * other.c,
            c * other.b + d * other.d
        };
    }
};

/* ¬озведение матрицы в степень */
Matrix2x2 power(Matrix2x2 base, uint64_t n) {
    Matrix2x2 result(1, 0, 0, 1); // identity
    while (n > 0) {
        if (n % 2 == 1)
            result = result * base;
        base = base * base;
        n /= 2;
    }
    return result;
}

std::string MatrixFibCounter::compute(const std::string& n_str) const {
    uint64_t n = std::stoull(n_str);
    if (n == 0) return "0";
    Matrix2x2 base(1, 1, 1, 0);
    Matrix2x2 result = power(base, n - 1);
    return std::to_string(result.a);
}