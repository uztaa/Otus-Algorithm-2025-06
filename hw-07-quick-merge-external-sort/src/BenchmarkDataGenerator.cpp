#include "BenchmarkDataGenerator.h"
#include "RandomArrayGenerator.h"
#include <algorithm>
#include <set>

constexpr uint32_t SEED = 12345;

std::vector<Record> BenchmarkDataGenerator::random(size_t size) {
    RandomArrayGenerator rg(SEED);
    return rg.regenerate(size, SEED);
}

std::vector<Record> BenchmarkDataGenerator::digits(size_t size) {
    // Используем RandomArrayGenerator с диапазоном 0..9
    RandomArrayGenerator dg(SEED, 0, 9);
    return dg.regenerate(size, SEED);
}

std::vector<Record> BenchmarkDataGenerator::revers(size_t size) {
    std::vector<Record> result;
    result.reserve(size);
    for (size_t i = size; i >= 1; --i) {
        result.emplace_back(static_cast<int>(i), "");
        if (i == 1) break; // чтобы избежать underflow у size_t
    }
    return result;
}

std::vector<Record> BenchmarkDataGenerator::sorted(size_t size, size_t dirties) {
    if (dirties > size) dirties = size;

    // 1. Сгенерируем отсортированный линейный массив 1...size
    std::vector<int> linearData(size);
    for (size_t i = 0; i < size; ++i) {
        linearData[i] = static_cast<int>(i + 1);
    }

    // 2. Генерируем уникальные индексы для "грязных" элементов (dirties)
    RandomArrayGenerator sg(SEED, 0, static_cast<int>(size - 1));

    std::set<int> uniqueIndices;
    while (uniqueIndices.size() < dirties) {
        auto candidates = sg.regenerate(dirties - uniqueIndices.size(), SEED);
        for (const auto& rec : candidates) {
            uniqueIndices.insert(rec.getKey());
            if (uniqueIndices.size() >= dirties) break;
        }
    }

    // 3. Меняем пары местами между индексами в uniqueIndices, перемешиваем элементы
    // Чтобы разнообразить «грязные» элементы, перевернём пары соседних индексов
    std::vector<int> indices(uniqueIndices.begin(), uniqueIndices.end());
    for (size_t i = 0; i + 1 < indices.size(); i += 2) {
        std::swap(linearData[indices[i]], linearData[indices[i + 1]]);
    }
    // Если нечетное число, последний остается без изменений

    // 4. Возвращаем результат в виде Record
    std::vector<Record> result;
    result.reserve(size);
    for (auto val : linearData) {
        result.emplace_back(val, "");
    }
    return result;
}
