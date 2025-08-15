#include "BinaryPowerCalculator.h"
#include <cstdlib>
#include <cmath>
#include <limits>

long double BinaryPowerCalculator::compute(const std::string& a_str, const std::string& n_str) const {
    long double a = std::stold(a_str);
    long long n = std::stoll(n_str);

    if (a == 0.0L && n == 0)
        return std::numeric_limits<long double>::quiet_NaN();
    if (a == 0.0L)
        return 0.0L;
    if (n == 0)
        return 1.0L;

    bool negative = n < 0;
    n = std::llabs(n);

    long double result = 1.0L;
    long double base = a;

    while (n > 0) {
        if (n & 1)
            result *= base;
        base *= base;
        n >>= 1;
    }

    return negative ? 1.0L / result : result;
}