#include "CountingSorter.hpp"
#include <algorithm>

template<typename T>
void CountingSorter::countingSort(std::vector<T>& arr, T maxValue) {
    if (arr.size() <= 1) {
        return;
    }

    // Создаём массив счётчиков
    std::vector<size_t> count(static_cast<size_t>(maxValue) + 1, 0);

    // Подсчитываем количество каждого значения
    for (const auto& val : arr) {
        ++count[val];
    }

    // Вычисляем кумулятивные суммы (позиции)
    for (size_t i = 1; i < count.size(); ++i) {
        count[i] += count[i - 1];
    }

    // Строим выходной массив (обход справа налево для стабильности)
    std::vector<T> output(arr.size());
    for (size_t i = arr.size(); i > 0; --i) {
        T val = arr[i - 1];
        output[count[val] - 1] = val;
        --count[val];
    }

    // Копируем результат обратно
    arr = std::move(output);
}

void CountingSorter::sort(std::vector<uint16_t>& arr) {
    if (arr.empty()) {
        return;
    }

    // Для 16-bit максимум всегда 65535
    countingSort<uint16_t>(arr, 65535);
}

void CountingSorter::sort(std::vector<uint32_t>& arr, uint32_t maxValue) {
    if (arr.empty()) {
        return;
    }

    // Если maxValue не задан, находим максимум
    if (maxValue == 0 && !arr.empty()) {
        maxValue = *std::max_element(arr.begin(), arr.end());
    }

    countingSort<uint32_t>(arr, maxValue);
}