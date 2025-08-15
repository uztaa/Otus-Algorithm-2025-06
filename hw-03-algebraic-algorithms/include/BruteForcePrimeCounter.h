#pragma once

#include "Counter.h"
#include <cstdint>

// Считает количество простых чисел от 1 до n (uint64_t), используя полный перебор делителей.
class BruteForcePrimeCounter : public Counter {
public:
	std::string compute(const std::string& n_str) const override;
private:
	bool is_prime(uint64_t n) const;
};