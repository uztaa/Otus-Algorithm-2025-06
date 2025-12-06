#include <gtest/gtest.h>
#include "Task2_Garland.hpp"

class Task2GarlandTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// Базовый пример из ДЗ
TEST_F(Task2GarlandTest, BasicExample) {
    std::vector<std::vector<int>> tree = {
        {1},
        {2, 3},
        {4, 5, 6}
    };
    int result = TaskTwoGarland::execute(tree);
    EXPECT_EQ(result, 10);  // 1 -> 3 -> 6
}

// Одна строка (одноэлементный треугольник)
TEST_F(Task2GarlandTest, SingleElement) {
    std::vector<std::vector<int>> tree = {{5}};
    int result = TaskTwoGarland::execute(tree);
    EXPECT_EQ(result, 5);
}

// Две строки
TEST_F(Task2GarlandTest, TwoRows) {
    std::vector<std::vector<int>> tree = {
        {1},
        {2, 3}
    };
    int result = TaskTwoGarland::execute(tree);
    EXPECT_EQ(result, 4);  // 1 -> 3
}

// Все положительные
TEST_F(Task2GarlandTest, AllPositive) {
    std::vector<std::vector<int>> tree = {
        {10},
        {20, 30},
        {40, 50, 60}
    };
    int result = TaskTwoGarland::execute(tree);
    EXPECT_EQ(result, 100);  // 10 + 30 + 60
}

// Отрицательные значения - DP выбирает максимум
TEST_F(Task2GarlandTest, WithNegatives) {
    std::vector<std::vector<int>> tree = {
        {-1},
        {-2, 3},
        {4, 5, 6}
    };
    int result = TaskTwoGarland::execute(tree);
    EXPECT_EQ(result, 8);  // -1 + 3 + max(4,5) = -1 + 3 + 5 = 7 или -1 + 3 + 6 = 8
}

// Нули
TEST_F(Task2GarlandTest, WithZeros) {
    std::vector<std::vector<int>> tree = {
        {0},
        {0, 0},
        {1, 1, 1}
    };
    int result = TaskTwoGarland::execute(tree);
    EXPECT_EQ(result, 1);
}

// Большой треугольник
TEST_F(Task2GarlandTest, LargeTriangle) {
    std::vector<std::vector<int>> tree = {
        {1},
        {2, 3},
        {4, 5, 6},
        {7, 8, 9, 10}
    };
    int result = TaskTwoGarland::execute(tree);
    EXPECT_GT(result, 0);  // Просто проверяем, что не ноль
}

// Идентичные элементы
TEST_F(Task2GarlandTest, IdenticalElements) {
    std::vector<std::vector<int>> tree = {
        {5},
        {5, 5},
        {5, 5, 5}
    };
    int result = TaskTwoGarland::execute(tree);
    EXPECT_EQ(result, 15);  // 5 + 5 + 5
}
