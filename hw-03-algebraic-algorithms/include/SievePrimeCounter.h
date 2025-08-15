#pragma once
#include "Counter.h"

// —читает количество простых чисел от 1 до n (uint64_t), использу€ решето Ёратосфена со сложностью O(n log log n).
class SievePrimeCounter : public Counter {
public:
    std::string compute(const std::string& n_str) const override;
};