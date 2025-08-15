# README

## Описание параметров AutoGenerateTest.cmake

auto_generate_tests(
DATA_DIR path/to/data <путь до папки с параметрами тестов: in, out-файлами>
CLASS_NAME MyClass <имя класса, который тестируется>
METHOD_NAME myMethod <имя метода, который тестируется>
OUTPUT_FILE path/to/output_test.cpp <путь до сгенерированного файла с тестами>
STRONG_ASSERT 1 (точность сравнения 1 - строгое равенство, 0 - сравнение с точностью EPS)
)

## Пример

auto_generate_tests(
DATA_DIR ${PROJECT_SOURCE_DIR}/test-resources/5.Primes
CLASS_NAME LinearSievePrimeCounter
METHOD_NAME compute
OUTPUT_FILE ${OUTPUT_TEST_DIR}/LinearSievePrimeCounterTests.cpp
STRONG_ASSERT 1
)