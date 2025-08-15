#pragma once
#include "PowerCalculator.h"

/* Сервис возведения в степень через степень двойки с домножением */
class DoublingPowerCalculator : public PowerCalculator {
public:
	long double compute(const std::string& a_str, const std::string& n_str) const override;
};