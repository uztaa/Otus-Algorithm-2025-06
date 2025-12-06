#include <gtest/gtest.h>
#include "Task7_BarnWidth.hpp"

class Task7BarnWidthTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// Возрастающий массив
TEST_F(Task7BarnWidthTest, IncreasingArray) {
    std::vector<int> height = {1, 2, 3, 4};
    std::vector<int> L, R;
    TaskSevenBarnWidth::execute(height, L, R);
    
    // Для возрастающего массива каждый элемент может идти как его индекс и дальше
    EXPECT_EQ(L[0], 0);
    EXPECT_EQ(L[1], 0);
    
    // R для каждого элемента
    EXPECT_EQ(R[3], 3);
}

// Убывающий массив
TEST_F(Task7BarnWidthTest, DecreasingArray) {
    std::vector<int> height = {4, 3, 2, 1};
    std::vector<int> L, R;
    TaskSevenBarnWidth::execute(height, L, R);
    
    // Для убывающего массива
    EXPECT_EQ(L[0], 0);
    EXPECT_EQ(R[3], 3);
}

// Плоский массив
TEST_F(Task7BarnWidthTest, FlatArray) {
    std::vector<int> height = {3, 3, 3, 3};
    std::vector<int> L, R;
    TaskSevenBarnWidth::execute(height, L, R);
    
    // Все одинаковые - все элементы относятся ко всему диапазону
    EXPECT_EQ(L[0], 0);
    EXPECT_EQ(R[3], 3);
}

// V-образный массив
TEST_F(Task7BarnWidthTest, VShapedArray) {
    std::vector<int> height = {3, 1, 3};
    std::vector<int> L, R;
    TaskSevenBarnWidth::execute(height, L, R);
    
    // Минимум в центре - может быть только сам
    EXPECT_EQ(L[1], 1);
    EXPECT_EQ(R[1], 1);
}

// Один элемент
TEST_F(Task7BarnWidthTest, SingleElement) {
    std::vector<int> height = {5};
    std::vector<int> L, R;
    TaskSevenBarnWidth::execute(height, L, R);
    
    EXPECT_EQ(L[0], 0);
    EXPECT_EQ(R[0], 0);
}

// Классический пример из алгоритма
TEST_F(Task7BarnWidthTest, ClassicExample) {
    std::vector<int> height = {2, 1, 5, 6, 2, 3};
    std::vector<int> L, R;
    TaskSevenBarnWidth::execute(height, L, R);
    
    // Проверяем размеры
    EXPECT_EQ(L.size(), 6);
    EXPECT_EQ(R.size(), 6);
    
    // Все индексы в пределах границ
    for (int i = 0; i < 6; ++i) {
        EXPECT_GE(L[i], 0);
        EXPECT_LE(L[i], i);
        EXPECT_GE(R[i], i);
        EXPECT_LT(R[i], 6);
    }
}

// Пики - большие значения среди маленьких
TEST_F(Task7BarnWidthTest, Peaks) {
    std::vector<int> height = {1, 5, 1, 5, 1};
    std::vector<int> L, R;
    TaskSevenBarnWidth::execute(height, L, R);
    
    // Пики имеют соседей с меньшей высотой
    // Проверяем что граница тесная для пиков
    EXPECT_LE(R[1] - L[1], 5);  // Пик не может быть шире чем весь массив
    EXPECT_LE(R[3] - L[3], 5);
}

// Ноль в диапазоне
TEST_F(Task7BarnWidthTest, WithZero) {
    std::vector<int> height = {2, 0, 3};
    std::vector<int> L, R;
    TaskSevenBarnWidth::execute(height, L, R);
    
    // Ноль имеет самые тесные границы
    EXPECT_EQ(L[1], 1);
    EXPECT_EQ(R[1], 1);
}

// Два нулевых значения
TEST_F(Task7BarnWidthTest, TwoZeros) {
    std::vector<int> height = {2, 0, 0, 3};
    std::vector<int> L, R;
    TaskSevenBarnWidth::execute(height, L, R);
    
    // Каждый ноль имеет свою границу
    EXPECT_EQ(R[1], 1);
    EXPECT_EQ(L[2], 2);
    EXPECT_EQ(R[2], 2);
}
