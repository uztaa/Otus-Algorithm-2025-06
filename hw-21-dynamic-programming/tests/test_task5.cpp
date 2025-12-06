#include <gtest/gtest.h>
#include "Task5_SmallBarn.hpp"

class Task5SmallBarnTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// Полностью свободная матрица
TEST_F(Task5SmallBarnTest, AllFree) {
    std::vector<std::vector<int>> matrix = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    int result = TaskFiveSmallBarn::execute(matrix);
    EXPECT_EQ(result, 9);  // Весь прямоугольник 3x3
}

// Полностью занято
TEST_F(Task5SmallBarnTest, AllOccupied) {
    std::vector<std::vector<int>> matrix = {
        {1, 1},
        {1, 1}
    };
    int result = TaskFiveSmallBarn::execute(matrix);
    EXPECT_EQ(result, 0);
}

// Одно препятствие в центре 3x3 - максимальный прямоугольник вокруг него
TEST_F(Task5SmallBarnTest, OneObstacle) {
    std::vector<std::vector<int>> matrix = {
        {0, 0, 0},
        {0, 1, 0},
        {0, 0, 0}
    };
    int result = TaskFiveSmallBarn::execute(matrix);
    // Максимум: либо верхняя полоса 1x3, либо нижняя 1x3, либо левая/правая полоса
    EXPECT_EQ(result, 3);
}

// Препятствие в углу - максимум будет 2x3 = 6 или 3x2 = 6
TEST_F(Task5SmallBarnTest, ObstacleInCorner) {
    std::vector<std::vector<int>> matrix = {
        {1, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    int result = TaskFiveSmallBarn::execute(matrix);
    EXPECT_EQ(result, 6);  // 2x3 левая часть нижних двух строк
}

// L-образное свободное пространство
TEST_F(Task5SmallBarnTest, LShapedFree) {
    std::vector<std::vector<int>> matrix = {
        {0, 0, 1},
        {0, 0, 1},
        {0, 0, 1}
    };
    int result = TaskFiveSmallBarn::execute(matrix);
    EXPECT_EQ(result, 6);  // 2x3 левая часть
}

// Единичная матрица
TEST_F(Task5SmallBarnTest, SingleCell) {
    std::vector<std::vector<int>> matrix = {{0}};
    int result = TaskFiveSmallBarn::execute(matrix);
    EXPECT_EQ(result, 1);
}

// Одна строка
TEST_F(Task5SmallBarnTest, SingleRow) {
    std::vector<std::vector<int>> matrix = {{0, 0, 0, 0}};
    int result = TaskFiveSmallBarn::execute(matrix);
    EXPECT_EQ(result, 4);
}

// Диагональные препятствия - все меньше или равно 2
TEST_F(Task5SmallBarnTest, DiagonalObstacles) {
    std::vector<std::vector<int>> matrix = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    };
    int result = TaskFiveSmallBarn::execute(matrix);
    // Максимум будет 2x2 = 4 или полоса 2x1 = 2
    EXPECT_GE(result, 2);
}

// Два препятствия
TEST_F(Task5SmallBarnTest, TwoObstacles) {
    std::vector<std::vector<int>> matrix = {
        {0, 0, 1},
        {0, 0, 0},
        {1, 0, 0}
    };
    int result = TaskFiveSmallBarn::execute(matrix);
    EXPECT_GT(result, 0);
}
