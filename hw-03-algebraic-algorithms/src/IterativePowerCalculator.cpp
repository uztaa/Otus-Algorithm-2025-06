#include "IterativePowerCalculator.h"
#include "StringToUint64Parser.h"
#include <cstdlib>
#include <cmath>
#include <limits>

long double IterativePowerCalculator::compute(const std::string& a_str, const std::string& n_str) const {
	long double a = std::stold(a_str);
	long long n = std::stoll(n_str);

	if (a == 0.0L && n == 0)
		return std::numeric_limits<long double>::quiet_NaN();
	if (a == 0.0L)
		return 0.0L;

	bool negative = n < 0;
	n = std::llabs(n);

	long double result = 1.0L;
	for (long long i = 0; i < n; ++i)
		result *= a;

	return negative ? 1.0L / result : result;
}