#pragma once
#include <cstdint>

// Вспомогательный класс для битовых операций
class BitUtils {
public:
    // Подсчёт количества установленных битов в числе
    static int popcount(uint64_t x) {
        int count = 0;
        while (x) {
            count += x & 1;
            x >>= 1;
        }
        return count;
    }
};