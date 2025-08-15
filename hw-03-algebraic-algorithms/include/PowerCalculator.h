#pragma once

#include <string>
#include <stdexcept>

/* Вычисляет степень числа a^n */
class PowerCalculator {
public:
	// a_str — основание (вещественное), n_str — степень (целое число)
	virtual long double compute(const std::string& a_str, const std::string& n_str) const {
		throw "PowerCalculator::compute not implemented";
	}

	~PowerCalculator() = default;
};