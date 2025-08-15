#pragma once
#include "Counter.h"

/* Рекурсивный (наивный, экспоненциальный) поиск числа Фиббоначи */
class RecursiveFibCounter : public Counter {
public:
	std::string compute(const std::string& n_str) const override;
};