#pragma once

#include <functional>
#include <string>

/**
 * @brief Простые хеш-функции для целочисленных ключей
 */

 /**
  * @brief Простая модульная хеш-функция
  */
template<typename T = int>
class SimpleHashFunction {
public:
    int operator()(T key, int capacity) const {
        return key % capacity;
    }
};

/**
 * @brief Мультипликативная хеш-функция
 */
template<typename T = int>
class MultiplicativeHashFunction {
private:
    static constexpr double GOLDEN_RATIO = 0.6180339887;
public:
    int operator()(T key, int capacity) const {
        double frac = (key * GOLDEN_RATIO);
        frac = frac - static_cast<int>(frac);
        return static_cast<int>(capacity * frac);
    }
};

/**
 * @brief Побитовая хеш-функция
 */
template<typename T = int>
class BitwiseHashFunction {
public:
    int operator()(T key, int capacity) const {
        unsigned int h = key;
        h ^= (h >> 16);
        h *= 0x7feb352d;
        h ^= (h >> 15);
        return static_cast<int>(h % capacity);
    }
};

/**
 * @brief Хеш-функция, похожая на ту, что используется в Java
 */
template<typename T = int>
class JavaLikeHashFunction {
public:
    int operator()(T key, int capacity) const {
        unsigned int h = key;
        h ^= (h >> 20) ^ (h >> 12);
        h ^= (h >> 7) ^ (h >> 4);
        return static_cast<int>(h % capacity);
    }
};

/**
 * @brief Полиномиальная хеш-функция
 */
template<typename T = int>
class PolynomialHashFunction {
private:
    static constexpr int BASE = 31;
public:
    int operator()(T key, int capacity) const {
        return (key * BASE) % capacity;
    }
};

/**
 * @brief Вторичная хеш-функция для открытой адресации
 */
template<typename T = int>
class SecondaryHashFunction {
public:
    int operator()(T key, int capacity) const {
        return 1 + (key % (capacity - 2));
    }
};
