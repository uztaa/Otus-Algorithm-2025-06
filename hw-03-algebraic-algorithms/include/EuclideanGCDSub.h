#pragma once

#include <string>

/* Вычисляет НОД двух целых чисел с помощью Алгоритма Эвклида с вычитанием */
class EuclideanGCDSub {
public:
    // calculate GCD
    std::string compute(const std::string& a_str, const std::string& b_str) const;
};