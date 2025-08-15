#include "RecursiveFibCounter.h"
#include <string>
#include <cstdint>

namespace {
	uint64_t fib(uint64_t n) {
		if (n <= 1) return n;
		return fib(n - 1) + fib(n - 2);
	}
}

std::string RecursiveFibCounter::compute(const std::string& n_str) const {
	uint64_t n = std::stoull(n_str);
	if (n > 40) throw std::invalid_argument("RecursiveFibCounter::Too slow for recursion");
	return std::to_string(fib(n));
}