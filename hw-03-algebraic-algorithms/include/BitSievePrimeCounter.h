#pragma once
#include "Counter.h"

// —читает количество простых чисел от 1 до n (uint64_t), использу€ битовую матрицу дл€ экономии пам€ти.
class BitSievePrimeCounter : public Counter {
public:
    std::string compute(const std::string& n_str) const override;
};