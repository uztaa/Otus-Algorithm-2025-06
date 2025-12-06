#include <gtest/gtest.h>
#include "Task6_BarnHeight.hpp"

class Task6BarnHeightTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// Пустая матрица без препятствий
TEST_F(Task6BarnHeightTest, NoObstacles) {
    std::vector<std::pair<int, int>> occupied;
    auto heights = TaskSixBarnHeight::execute(3, 3, occupied);
    
    // Первая строка: все 1
    EXPECT_EQ(heights[0][0], 1);
    EXPECT_EQ(heights[0][1], 1);
    EXPECT_EQ(heights[0][2], 1);
    
    // Последняя строка: все 3
    EXPECT_EQ(heights[2][0], 3);
    EXPECT_EQ(heights[2][1], 3);
    EXPECT_EQ(heights[2][2], 3);
}

// Одно препятствие
TEST_F(Task6BarnHeightTest, OneObstacle) {
    std::vector<std::pair<int, int>> occupied = {{1, 1}};
    auto heights = TaskSixBarnHeight::execute(3, 3, occupied);
    
    // Препятствие имеет высоту 0
    EXPECT_EQ(heights[1][1], 0);
    
    // Над препятствием: высота 1 (только сама верхняя ячейка)
    EXPECT_EQ(heights[0][1], 1);
    
    // Под препятствием: высота 1 (сбрасывается после препятствия)
    EXPECT_EQ(heights[2][1], 1);
}

// Препятствие в первой строке
TEST_F(Task6BarnHeightTest, ObstacleInFirstRow) {
    std::vector<std::pair<int, int>> occupied = {{0, 1}};
    auto heights = TaskSixBarnHeight::execute(3, 3, occupied);
    
    EXPECT_EQ(heights[0][1], 0);
    EXPECT_EQ(heights[1][1], 1);
    EXPECT_EQ(heights[2][1], 2);
}

// Столб препятствий
TEST_F(Task6BarnHeightTest, ColumnOfObstacles) {
    std::vector<std::pair<int, int>> occupied = {
        {0, 1},
        {1, 1},
        {2, 1}
    };
    auto heights = TaskSixBarnHeight::execute(3, 3, occupied);
    
    // Весь столб - нули
    EXPECT_EQ(heights[0][1], 0);
    EXPECT_EQ(heights[1][1], 0);
    EXPECT_EQ(heights[2][1], 0);
}

// Строка препятствий
TEST_F(Task6BarnHeightTest, RowOfObstacles) {
    std::vector<std::pair<int, int>> occupied = {
        {1, 0},
        {1, 1},
        {1, 2}
    };
    auto heights = TaskSixBarnHeight::execute(3, 3, occupied);
    
    // Над строкой: высота 1
    EXPECT_EQ(heights[0][0], 1);
    EXPECT_EQ(heights[0][1], 1);
    EXPECT_EQ(heights[0][2], 1);
    
    // Сама строка: нули
    EXPECT_EQ(heights[1][0], 0);
    EXPECT_EQ(heights[1][1], 0);
    EXPECT_EQ(heights[1][2], 0);
    
    // Под строкой: высота 1
    EXPECT_EQ(heights[2][0], 1);
    EXPECT_EQ(heights[2][1], 1);
    EXPECT_EQ(heights[2][2], 1);
}

// Лестница препятствий
TEST_F(Task6BarnHeightTest, StairObstacles) {
    std::vector<std::pair<int, int>> occupied = {
        {0, 0},
        {1, 1},
        {2, 2}
    };
    auto heights = TaskSixBarnHeight::execute(3, 3, occupied);
    
    EXPECT_EQ(heights[0][0], 0);  // Препятствие
    EXPECT_EQ(heights[1][1], 0);  // Препятствие
    EXPECT_EQ(heights[2][2], 0);  // Препятствие
}

// Размер 1x1
TEST_F(Task6BarnHeightTest, SingleCell) {
    std::vector<std::pair<int, int>> occupied;
    auto heights = TaskSixBarnHeight::execute(1, 1, occupied);
    
    EXPECT_EQ(heights[0][0], 1);
}

// Препятствие за пределами матрицы (должно быть игнорировано)
TEST_F(Task6BarnHeightTest, OutOfBoundsObstacle) {
    std::vector<std::pair<int, int>> occupied = {{5, 5}};  // За пределами 3x3
    auto heights = TaskSixBarnHeight::execute(3, 3, occupied);
    
    // Всё должно считаться нормально
    EXPECT_EQ(heights[2][2], 3);  // Максимум для 3x3
}
