#pragma once
#include "Counter.h"

/* Итеративный(линейный) поиск числа Фиббоначи */
class IterativeFibCounter : public Counter {
public:
	std::string compute(const std::string& n_str) const override;
};