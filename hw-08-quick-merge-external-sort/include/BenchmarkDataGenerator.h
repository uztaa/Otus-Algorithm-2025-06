#pragma once

#include <vector>
#include "Record.h"

class BenchmarkDataGenerator {
public:
    // Генерация случайных чисел (standard random)
    std::vector<Record> random(size_t size);

    // Генерация случайных цифр (ключи от 0 до 9)
    std::vector<Record> digits(size_t size);

    // Обратно отсортированный массив: от size до 1
    std::vector<Record> revers(size_t size);

    // Отсортированный на 99% массив с dirties "грязными" элементами
    std::vector<Record> sorted(size_t size, size_t dirties);
};
