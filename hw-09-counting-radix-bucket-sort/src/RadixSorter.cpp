#include "RadixSorter.hpp"
#include <algorithm>

RadixSorter::RadixSorter(uint32_t base) : base_(base) {}

template<typename T>
void RadixSorter::countingSortByDigit(std::vector<T>& arr, T exp) {
    const size_t n = arr.size();
    std::vector<T> output(n);
    std::vector<size_t> count(base_, 0);

    // Подсчёт по текущему разряду
    for (size_t i = 0; i < n; ++i) {
        size_t digit = (arr[i] / exp) % base_;
        ++count[digit];
    }

    // Кумулятивные суммы
    for (size_t i = 1; i < base_; ++i) {
        count[i] += count[i - 1];
    }

    // Построение output (справа налево для стабильности)
    for (size_t i = n; i > 0; --i) {
        size_t digit = (arr[i - 1] / exp) % base_;
        output[count[digit] - 1] = arr[i - 1];
        --count[digit];
    }

    arr = std::move(output);
}

void RadixSorter::sort(std::vector<uint16_t>& arr) {
    if (arr.size() <= 1) {
        return;
    }

    // Для 16-bit чисел с base=256: 2 прохода
    // exp = 1: младший байт
    // exp = 256: старший байт
    uint16_t exp = 1;
    while (exp <= 256) {  // 65535 / 256 = 255, нужен второй проход
        countingSortByDigit<uint16_t>(arr, exp);
        exp *= static_cast<uint16_t>(base_);
        if (exp == 0) break;  // Overflow protection
    }
}

void RadixSorter::sort(std::vector<uint32_t>& arr, uint32_t maxValue) {
    if (arr.size() <= 1) {
        return;
    }

    // Находим максимум для определения числа разрядов
    if (maxValue == 0) {
        maxValue = *std::max_element(arr.begin(), arr.end());
    }

    // Сортируем по каждому разряду
    for (uint32_t exp = 1; maxValue / exp > 0; exp *= base_) {
        countingSortByDigit<uint32_t>(arr, exp);
    }
}