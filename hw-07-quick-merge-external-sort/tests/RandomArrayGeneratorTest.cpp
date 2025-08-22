#include <gtest/gtest.h>
#include "RandomArrayGenerator.h"

// Проверка, что generate(N) каждый раз создает разные массивы
TEST(RandomArrayGeneratorTest, GenerateProducesDifferentArrays) {
    RandomArrayGenerator generator(12345L);

    auto arr1 = generator.generate(1000);
    auto arr2 = generator.generate(1000);

    // Размеры должны совпадать
    ASSERT_EQ(arr1.size(), arr2.size());

    // Считаем, что вероятность совпадения всех элементов в двух разных генерациях мала,
    // значит массивы должны отличаться хотя бы в одном элементе
    bool different_found = false;
    for (size_t i = 0; i < arr1.size(); ++i) {
        if (arr1[i].getKey() != arr2[i].getKey()) {
            different_found = true;
            break;
        }
    }

    EXPECT_TRUE(different_found) << "Два подряд сгенерированных массива одинаковы";
}

// Проверка, что regenerate(N, seed) с фиксированным seed каждый раз создает одинаковый массив
TEST(RandomArrayGeneratorTest, RegenerateProducesSameArrayWithSameSeed) {
    RandomArrayGenerator generator;

    uint32_t seed = 12345L;
    size_t size = 100;

    auto arr1 = generator.regenerate(size, seed);
    auto arr2 = generator.regenerate(size, seed);

    // Размеры совпадают
    ASSERT_EQ(arr1.size(), arr2.size());

    // Все элементы совпадают
    for (size_t i = 0; i < size; ++i) {
        EXPECT_EQ(arr1[i].getKey(), arr2[i].getKey()) << "Элемент " << i << " не совпадает";
        EXPECT_EQ(arr1[i].getValue(), arr2[i].getValue()) << "Значение элемента " << i << " не совпадает";
    }
}
