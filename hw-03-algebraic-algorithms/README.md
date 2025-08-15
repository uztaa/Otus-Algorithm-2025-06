# HW-03: Алгебраические алгоритмы

## Структура проекта

cmake - папка с cmake-файлом для автоматичсекой генерации GoogleTest-ов из параметров тестов "in" и "out"-файлов
include - папка с заголовочными файлами и интерфейсами (абстрактыми классами)
src - реализации классов .cpp
test-resources - содердит папки с параметрами тестов "in" и "out"-файлы
test - папка для генерации тестов и запуска тестов

## Как генерируеются файлы

Например, для BinaryPowerCalculator (Сервис возведения в степень через двоичное разложение показателя степени) вот такой код для генерации тестов:

### код в CMakeLists.txt для генерации тестов для BinaryPowerCalculator
auto_generate_tests(
DATA_DIR ${PROJECT_SOURCE_DIR}/test-resources/3.Power
CLASS_NAME BinaryPowerCalculator
METHOD_NAME compute
OUTPUT_FILE ${OUTPUT_TEST_DIR}/BinaryPowerCalculatorTests.cpp
STRONG_ASSERT 0
)

### Сгенерированный файл с тестами

#include <gtest/gtest.h>
#include "MathSettings.h"
#include "BinaryPowerCalculator.h"

TEST(BinaryPowerCalculatorTest, Test0) {
    BinaryPowerCalculator service;
    EXPECT_NEAR(service.compute("2", "10"), 1024.0, EPS);
}

TEST(BinaryPowerCalculatorTest, Test1) {
    BinaryPowerCalculator service;
    EXPECT_NEAR(service.compute("123456789", "0"), 1.0, EPS);
}

TEST(BinaryPowerCalculatorTest, Test2) {
    BinaryPowerCalculator service;
    EXPECT_NEAR(service.compute("1.001", "1000"), 2.71692393224, EPS);
}

TEST(BinaryPowerCalculatorTest, Test3) {
    BinaryPowerCalculator service;
    EXPECT_NEAR(service.compute("1.0001", "10000"), 2.71814592682, EPS);
}

TEST(BinaryPowerCalculatorTest, Test4) {
    BinaryPowerCalculator service;
    EXPECT_NEAR(service.compute("1.00001", "100000"), 2.71826823719, EPS);
}

TEST(BinaryPowerCalculatorTest, Test5) {
    BinaryPowerCalculator service;
    EXPECT_NEAR(service.compute("1.000001", "1000000"), 2.7182804691, EPS);
}

TEST(BinaryPowerCalculatorTest, Test6) {
    BinaryPowerCalculator service;
    EXPECT_NEAR(service.compute("1.0000001", "10000000"), 2.71828169413, EPS);
}
