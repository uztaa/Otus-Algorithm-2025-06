#pragma once
#include "PowerCalculator.h"

/* Сервис возведения в степень через обычные итерации */
class IterativePowerCalculator : public PowerCalculator {
public:
	long double compute(const std::string& a_str, const std::string& n_str) const override;
};