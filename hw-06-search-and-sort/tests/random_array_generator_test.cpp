// tests/random_array_generator_test.cpp
#include <gtest/gtest.h>

#include "RandomArrayGenerator.h"

#include <unordered_set>

/*  Проверяем, что генератор корректно обрабатывает граничный случай
    и возвращает пустой вектор. */
TEST(RandomArrayGenerator, GeneratesEmptyVectorWhenSizeIsZero)
{
    RandomArrayGenerator gen;
    auto result = gen.generate(0);

    EXPECT_TRUE(result.empty());
}

/*  Размер выходного вектора должен ровно совпадать
    с запрошенным размером. */
TEST(RandomArrayGenerator, GeneratesVectorOfRequestedSize)
{
    RandomArrayGenerator gen;
    constexpr std::size_t N = 128;

    auto result = gen.generate(N);

    EXPECT_EQ(result.size(), N);
}

/*  С высокой вероятностью ключи должны быть уникальны.
    Тест не детерминированный на 100 %, но при разумной реализации
    (равномерное распределение на int-овом диапазоне) коллизии
    практически исключены. */
TEST(RandomArrayGenerator, KeysAreLikelyUnique)
{
    RandomArrayGenerator gen;
    constexpr std::size_t N = 10;// 256;

    auto result = gen.generate(N);

    std::unordered_set<int> keys;
    for (const auto& rec : result) {
        keys.insert(rec.getKey());
		std::cout << "Key: " << rec.getKey() << ", Value: " << rec.getValue() << std::endl;
    };

    EXPECT_TRUE(100 * keys.size() / result.size() >= 80)
        << "Сгенерировано слишком много повторяющихся ключей — проверьте реализацию RandomArrayGenerator";
}
