#pragma once

#include <cstdint>
#include <cstddef>

/**
 * @brief Утилиты для работы с битами и хэшами.
 * 
 * Содержит вспомогательные функции, не зависящие от конкретной
 * реализации хэш-функции.
 */
namespace HashUtils {

/**
 * @brief Подсчёт ведущих нулей в 64-битном числе.
 * 
 * Используется в HyperLogLog для определения позиции первой единицы.
 * 
 * @param x 64-битное число
 * @return Количество ведущих нулевых битов (0-64)
 */
inline int countLeadingZeros(uint64_t x) {
    if (x == 0) return 64;
    
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_clzll(x);
#elif defined(_MSC_VER)
    unsigned long index;
    _BitScanReverse64(&index, x);
    return 63 - static_cast<int>(index);
#else
    // Портативная реализация
    int count = 0;
    if ((x & 0xFFFFFFFF00000000ULL) == 0) { count += 32; x <<= 32; }
    if ((x & 0xFFFF000000000000ULL) == 0) { count += 16; x <<= 16; }
    if ((x & 0xFF00000000000000ULL) == 0) { count += 8;  x <<= 8;  }
    if ((x & 0xF000000000000000ULL) == 0) { count += 4;  x <<= 4;  }
    if ((x & 0xC000000000000000ULL) == 0) { count += 2;  x <<= 2;  }
    if ((x & 0x8000000000000000ULL) == 0) { count += 1; }
    return count;
#endif
}

/**
 * @brief Подсчёт позиции первой единицы (1-based).
 * 
 * Возвращает позицию самого левого бита, равного 1.
 * Если x = 0, возвращает maxBits + 1.
 * 
 * Пример:
 * - x = 0b10000000... → позиция 1
 * - x = 0b01000000... → позиция 2
 * - x = 0b00100000... → позиция 3
 * 
 * @param x 64-битное число
 * @param maxBits Максимальное количество бит для анализа
 * @return Позиция первой единицы (1-based), или maxBits+1 если все нули
 */
inline int firstOnePosition(uint64_t x, int maxBits = 64) {
    int zeros = countLeadingZeros(x);
    if (zeros >= maxBits) {
        return maxBits + 1;
    }
    return zeros + 1;
}

/**
 * @brief Подсчёт установленных битов (popcount).
 */
inline int popcount(uint64_t x) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x);
#else
    // Портативная реализация
    x = x - ((x >> 1) & 0x5555555555555555ULL);
    x = (x & 0x3333333333333333ULL) + ((x >> 2) & 0x3333333333333333ULL);
    x = (x + (x >> 4)) & 0x0f0f0f0f0f0f0f0fULL;
    return static_cast<int>((x * 0x0101010101010101ULL) >> 56);
#endif
}

}  // namespace HashUtils
