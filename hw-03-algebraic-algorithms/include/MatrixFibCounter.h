#pragma once
#include "Counter.h"

/* Вычисление числа Фиббоначчи путем возведения матрицы в степень */
class MatrixFibCounter : public Counter {
public:
	std::string compute(const std::string& n_str) const override;
};