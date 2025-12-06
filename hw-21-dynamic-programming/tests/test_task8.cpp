#include <gtest/gtest.h>
#include "Task8_LargeBarn.hpp"

class Task8LargeBarnTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// Полностью свободная матрица 3x3 - должна вернуть 9
TEST_F(Task8LargeBarnTest, AllFree) {
    std::vector<std::pair<int, int>> occupied;
    int result = TaskEightLargeBarn::execute(3, 3, occupied);
    // Строка 0: высоты [1,1,1] → макс = 3 (1×3)
    // Строка 1: высоты [2,2,2] → макс = 6 (2×3)
    // Строка 2: высоты [3,3,3] → макс = 9 (3×3) ← МАКСИМУМ
    EXPECT_EQ(result, 9);
}

// Полностью занято
TEST_F(Task8LargeBarnTest, AllOccupied) {
    std::vector<std::pair<int, int>> occupied = {
        {0, 0}, {0, 1}, {0, 2},
        {1, 0}, {1, 1}, {1, 2},
        {2, 0}, {2, 1}, {2, 2}
    };
    int result = TaskEightLargeBarn::execute(3, 3, occupied);
    EXPECT_EQ(result, 0);
}

// Одно препятствие в центре
TEST_F(Task8LargeBarnTest, OneObstacleCenter) {
    std::vector<std::pair<int, int>> occupied = {{1, 1}};
    int result = TaskEightLargeBarn::execute(3, 3, occupied);
    // Строка 0: [1,1,1] → 3
    // Строка 1: [2,0,2] → макс = 2 (1×2 слева или справа)
    // Строка 2: [3,1,3] → макс = 3
    EXPECT_EQ(result, 3);
}

// Горизонтальная линия препятствий в строке 1
TEST_F(Task8LargeBarnTest, HorizontalLine) {
    std::vector<std::pair<int, int>> occupied = {
        {1, 0}, {1, 1}, {1, 2}
    };
    int result = TaskEightLargeBarn::execute(3, 3, occupied);
    // Строка 0: [1,1,1] → макс = 3 (1×3)
    // Строка 1: [0,0,0] → макс = 0
    // Строка 2: [1,1,1] → макс = 3 (1×3)
    // Общий максимум = 3
    EXPECT_EQ(result, 3);
}

// Вертикальная линия препятствий в столбце 1
TEST_F(Task8LargeBarnTest, VerticalLine) {
    std::vector<std::pair<int, int>> occupied = {
        {0, 1}, {1, 1}, {2, 1}
    };
    int result = TaskEightLargeBarn::execute(3, 3, occupied);
    // Строка 0: [1,0,1] → макс = 1 (не можем соединить через 0)
    // Строка 1: [2,0,2] → макс = 2 (1 ячейка × 2 высоты)
    // Строка 2: [3,0,3] → макс = 3 (1×3 слева ИЛИ 1×3 справа)
    EXPECT_EQ(result, 3);
}

// Диагональная линия препятствий
TEST_F(Task8LargeBarnTest, DiagonalLine) {
    std::vector<std::pair<int, int>> occupied = {
        {0, 0}, {1, 1}, {2, 2}
    };
    int result = TaskEightLargeBarn::execute(3, 3, occupied);
    EXPECT_GT(result, 0);
}

// Один квадрат свободы
TEST_F(Task8LargeBarnTest, SingleCell) {
    std::vector<std::pair<int, int>> occupied;
    int result = TaskEightLargeBarn::execute(1, 1, occupied);
    EXPECT_EQ(result, 1);
}

// Две части матрицы - препятствие справа
TEST_F(Task8LargeBarnTest, TwoRegions) {
    std::vector<std::pair<int, int>> occupied = {
        {0, 2}, {1, 2}, {2, 2}
    };
    int result = TaskEightLargeBarn::execute(3, 3, occupied);
    // Строка 0: [1,1,0] → макс = 2 (1×2)
    // Строка 1: [2,2,0] → макс = 4 (2×2)
    // Строка 2: [3,3,0] → макс = 6 (3×2) ← МАКСИМУМ
    EXPECT_EQ(result, 6);
}

// Шахматный паттерн
TEST_F(Task8LargeBarnTest, CheckerboardPattern) {
    std::vector<std::pair<int, int>> occupied = {
        {0, 0}, {0, 2},
        {2, 0}, {2, 2}
    };
    int result = TaskEightLargeBarn::execute(3, 3, occupied);
    EXPECT_GT(result, 0);
}

// Большая матрица 10x10 с одним препятствием
TEST_F(Task8LargeBarnTest, LargeMatrixSmallObstacle) {
    std::vector<std::pair<int, int>> occupied = {{5, 5}};
    int result = TaskEightLargeBarn::execute(10, 10, occupied);
    // Максимум будет на строке 9 (перед низом)
    // где все высоты достаточно большие
    // Или может быть 5×10 вверху = 50 (первые 5 строк)
    EXPECT_GE(result, 45);  // Должен быть довольно большой
}

// Градиент препятствий
TEST_F(Task8LargeBarnTest, GradientPattern) {
    std::vector<std::pair<int, int>> occupied = {
        {0, 2}, {1, 2}, {2, 2},
        {2, 0}, {2, 1}, {2, 2}
    };
    int result = TaskEightLargeBarn::execute(3, 3, occupied);
    // L-образная свободная область
    EXPECT_GT(result, 0);
}

// Прямоугольная матрица 2×5
TEST_F(Task8LargeBarnTest, RectangularMatrix) {
    std::vector<std::pair<int, int>> occupied;
    int result = TaskEightLargeBarn::execute(2, 5, occupied);
    // Строка 0: [1,1,1,1,1] → макс = 5 (1×5)
    // Строка 1: [2,2,2,2,2] → макс = 10 (2×5) ← МАКСИМУМ
    EXPECT_EQ(result, 10);
}
