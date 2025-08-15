#include "IterativeFibCounter.h"
#include <string>
#include <cstdint>

std::string IterativeFibCounter::compute(const std::string& n_str) const {
	uint64_t n = std::stoull(n_str);
	if (n == 0) return "0";
	if (n == 1) return "1";

	uint64_t a = 0, b = 1, temp;
	for (uint64_t i = 2; i <= n; ++i) {
		temp = a + b;
		a = b;
		b = temp;
	}

	return std::to_string(b);
}