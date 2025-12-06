#include <gtest/gtest.h>
#include "Task1_Fractions.hpp"

class Task1FractionsTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// Базовые примеры
TEST_F(Task1FractionsTest, BasicExample1) {
    std::string result = TaskOneFractions::execute("1/2+1/3");
    EXPECT_EQ(result, "5/6");
}

TEST_F(Task1FractionsTest, BasicExample2) {
    std::string result = TaskOneFractions::execute("1/4+1/4");
    EXPECT_EQ(result, "1/2");
}

// Одинаковые дроби
TEST_F(Task1FractionsTest, IdenticalFractions) {
    std::string result = TaskOneFractions::execute("1/5+1/5");
    EXPECT_EQ(result, "2/5");
}

// Сумма дающая целое число
TEST_F(Task1FractionsTest, ResultWhole) {
    std::string result = TaskOneFractions::execute("2/3+1/3");
    EXPECT_EQ(result, "1/1");
}

// Большие числители/знаменатели
TEST_F(Task1FractionsTest, LargeNumbers) {
    std::string result = TaskOneFractions::execute("1000/3000+1000/3000");
    EXPECT_EQ(result, "2/3");
}

// Сокращение сложной дроби
TEST_F(Task1FractionsTest, ComplexReduction) {
    std::string result = TaskOneFractions::execute("1/6+1/4");
    EXPECT_EQ(result, "5/12");
}

// Разные знаменатели
TEST_F(Task1FractionsTest, DifferentDenominators) {
    std::string result = TaskOneFractions::execute("1/2+2/5");
    EXPECT_EQ(result, "9/10");
}

// Один, два - один (граничный случай)
TEST_F(Task1FractionsTest, MinimalFraction) {
    std::string result = TaskOneFractions::execute("1/1+1/1");
    EXPECT_EQ(result, "2/1");
}
