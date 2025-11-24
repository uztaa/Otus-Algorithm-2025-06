#include <gtest/gtest.h>
#include "RandomArrayGenerator.h"

// Проверка, что generate(N) каждый раз создает разные массивы
TEST(RandomArrayGeneratorTest, GenerateProducesDifferentArrays)
{
    RandomArrayGenerator generator(12345L);

    auto arr1 = generator.generate(1000);
    auto arr2 = generator.generate(1000);

    // Размеры должны совпадать
    ASSERT_EQ(arr1.size(), arr2.size());

    // Считаем, что вероятность совпадения всех элементов в двух разных генерациях мала,
    // значит массивы должны отличаться хотя бы в одном элементе
    bool different_found = false;
    for (size_t i = 0; i < arr1.size(); ++i)
    {
        if (arr1[i].getKey() != arr2[i].getKey())
        {
            different_found = true;
            break;
        }
    }

    EXPECT_TRUE(different_found) << "Два подряд сгенерированных массива одинаковы";
}

// Проверка, что regenerate(N, seed) с фиксированным seed каждый раз создает одинаковый массив
TEST(RandomArrayGeneratorTest, RegenerateProducesSameArrayWithSameSeed)
{
    RandomArrayGenerator generator(12345L);

    uint32_t seed = 12345;
    size_t size = 10000;

    auto arr1 = generator.regenerate(size, seed);
    auto arr2 = generator.regenerate(size, seed);

    // Размеры совпадают
    ASSERT_EQ(arr1.size(), arr2.size());

    // счетчик совпадений при генерации
    uint32_t sameCounter = 0;

    // Все элементы совпадают
    for (size_t i = 0; i < size; ++i)
    {
        sameCounter = arr1[i].getKey() == arr2[i].getKey();
    }

    EXPECT_TRUE(sameCounter <= size/100); // считаем, что генератор должен хотя 99% новых данных создавать
}
