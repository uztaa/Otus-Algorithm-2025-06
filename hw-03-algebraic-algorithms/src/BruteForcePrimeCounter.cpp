#include "BruteForcePrimeCounter.h"
#include <string>
#include <cstdlib>

bool BruteForcePrimeCounter::is_prime(uint64_t n) const {
	if (n <= 1) return false;
	for (uint64_t d = 2; d < n; ++d)
		if (n % d == 0) return false;
	return true;
}

std::string BruteForcePrimeCounter::compute(const std::string& n_str) const {
	uint64_t n = std::stoll(n_str);
	uint64_t count = 0;
	for (uint64_t i = 2; i <= n; ++i)
		if (is_prime(i)) {
			++count;
		}
	return std::to_string(count);
}