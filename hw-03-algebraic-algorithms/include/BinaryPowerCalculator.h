#pragma once
#include "PowerCalculator.h"

// Сервис возведения в степень через двоичное разложение показателя степени
class BinaryPowerCalculator : public PowerCalculator {
public:
	long double compute(const std::string& a_str, const std::string& n_str) const override;
};