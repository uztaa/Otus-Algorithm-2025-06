#pragma once

#include <string>

/* Вычисляет НОД двух целых чисел с помощью Алгоритма Стейница (бинарный алгоритм вычисления НОД) */
class SteinGCD {
public:
	// Вычисляет НОД двух чисел, заданных строками
	std::string compute(const std::string& a_str, const std::string& b_str) const;
};