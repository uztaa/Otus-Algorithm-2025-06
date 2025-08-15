#pragma once
#include "Counter.h"

/* Вычисление числа Фиббоначчи через формулу Бине (золотого сечения, приближённый) */
class GoldenRatioFibCounter : public Counter {
public:
	std::string compute(const std::string& n_str) const override;
};