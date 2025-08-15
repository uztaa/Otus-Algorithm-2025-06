#pragma once
#include "Counter.h"

// Считает количество простых чисел от 1 до n (uint64_t), используя оптимизацию до √i.
class SqrtOptimizedPrimeCounter : public Counter {
public:
    std::string compute(const std::string& n_str) const override;
};