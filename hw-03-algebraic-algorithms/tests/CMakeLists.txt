set(TEST_SOURCES
    "all_test_runner.cpp"
)

set(OUTPUT_TEST_DIR ${CMAKE_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${OUTPUT_TEST_DIR})

# include test-generator
include(${PROJECT_SOURCE_DIR}/cmake/AutoGenerateTests.cmake)

# >>> START test-generation

# auto-test generator for EuclideanGCDSub
auto_generate_tests(
DATA_DIR ${PROJECT_SOURCE_DIR}/test-resources/2.GCD
CLASS_NAME EuclideanGCDSub
METHOD_NAME compute
OUTPUT_FILE ${OUTPUT_TEST_DIR}/EuclideanGCDSubTests.cpp
STRONG_ASSERT 1
)

# auto-test generator for EuclidianGCDDiv
auto_generate_tests(
DATA_DIR ${PROJECT_SOURCE_DIR}/test-resources/2.GCD
CLASS_NAME EuclidianGCDDiv
METHOD_NAME compute
OUTPUT_FILE ${OUTPUT_TEST_DIR}/EuclidianGCDDivTests.cpp
STRONG_ASSERT 1
)

# auto-test generator for SteinGCD
auto_generate_tests(
DATA_DIR ${PROJECT_SOURCE_DIR}/test-resources/2.GCD
CLASS_NAME SteinGCD
METHOD_NAME compute
OUTPUT_FILE ${OUTPUT_TEST_DIR}/SteinGCDTests.cpp
STRONG_ASSERT 1
)

# auto-test generator for IterativePowerCalculator
auto_generate_tests(
DATA_DIR ${PROJECT_SOURCE_DIR}/test-resources/3.Power
CLASS_NAME IterativePowerCalculator
METHOD_NAME compute
OUTPUT_FILE ${OUTPUT_TEST_DIR}/IterativePowerCalculatorTests.cpp
STRONG_ASSERT 0
)

# auto-test generator for DoublingPowerCalculator
auto_generate_tests(
DATA_DIR ${PROJECT_SOURCE_DIR}/test-resources/3.Power
CLASS_NAME DoublingPowerCalculator
METHOD_NAME compute
OUTPUT_FILE ${OUTPUT_TEST_DIR}/DoublingPowerCalculatorTests.cpp
STRONG_ASSERT 0
)

# auto-test generator for BinaryPowerCalculator
auto_generate_tests(
DATA_DIR ${PROJECT_SOURCE_DIR}/test-resources/3.Power
CLASS_NAME BinaryPowerCalculator
METHOD_NAME compute
OUTPUT_FILE ${OUTPUT_TEST_DIR}/BinaryPowerCalculatorTests.cpp
STRONG_ASSERT 0
)

# auto-test generator for RecursiveFibCounter
auto_generate_tests(
DATA_DIR ${PROJECT_SOURCE_DIR}/test-resources/4.Fibo
CLASS_NAME RecursiveFibCounter
METHOD_NAME compute
OUTPUT_FILE ${OUTPUT_TEST_DIR}/RecursiveFibCounterTests.cpp
STRONG_ASSERT 1
)

# auto-test generator for IterativeFibCounter
auto_generate_tests(
DATA_DIR ${PROJECT_SOURCE_DIR}/test-resources/4.Fibo
CLASS_NAME IterativeFibCounter
METHOD_NAME compute
OUTPUT_FILE ${OUTPUT_TEST_DIR}/IterativeFibCounterTests.cpp
STRONG_ASSERT 1
)

#  auto-test generator for GoldenRatioFibCounter
auto_generate_tests(
DATA_DIR ${PROJECT_SOURCE_DIR}/test-resources/4.Fibo
CLASS_NAME GoldenRatioFibCounter
METHOD_NAME compute
OUTPUT_FILE ${OUTPUT_TEST_DIR}/GoldenRatioFibCounterTests.cpp
STRONG_ASSERT 1
)

# auto-test generator for MatrixFibCounter
auto_generate_tests(
DATA_DIR ${PROJECT_SOURCE_DIR}/test-resources/4.Fibo
CLASS_NAME MatrixFibCounter
METHOD_NAME compute
OUTPUT_FILE ${OUTPUT_TEST_DIR}/MatrixFibCounterTests.cpp
STRONG_ASSERT 1
)

# auto-test generator for BruteForcePrimeCounter
auto_generate_tests(
DATA_DIR ${PROJECT_SOURCE_DIR}/test-resources/5.Primes
CLASS_NAME BruteForcePrimeCounter
METHOD_NAME compute
OUTPUT_FILE ${OUTPUT_TEST_DIR}/BruteForcePrimeCounterTests.cpp
STRONG_ASSERT 1
)

# auto-test generator for SqrtOptimizedPrimeCounter
auto_generate_tests(
DATA_DIR ${PROJECT_SOURCE_DIR}/test-resources/5.Primes
CLASS_NAME SqrtOptimizedPrimeCounter
METHOD_NAME compute
OUTPUT_FILE ${OUTPUT_TEST_DIR}/SqrtOptimizedPrimeCounterTests.cpp
STRONG_ASSERT 1
)

# auto-test generator for SievePrimeCounter
auto_generate_tests(
DATA_DIR ${PROJECT_SOURCE_DIR}/test-resources/5.Primes
CLASS_NAME SievePrimeCounter
METHOD_NAME compute
OUTPUT_FILE ${OUTPUT_TEST_DIR}/SievePrimeCounterTests.cpp
STRONG_ASSERT 1
)

# auto-test generator for BitSievePrimeCounter
auto_generate_tests(
DATA_DIR ${PROJECT_SOURCE_DIR}/test-resources/5.Primes
CLASS_NAME BitSievePrimeCounter
METHOD_NAME compute
OUTPUT_FILE ${OUTPUT_TEST_DIR}/BitSievePrimeCounterTests.cpp
STRONG_ASSERT 1
)

# auto-test generator for LinearSievePrimeCounter
auto_generate_tests(
DATA_DIR ${PROJECT_SOURCE_DIR}/test-resources/5.Primes
CLASS_NAME LinearSievePrimeCounter
METHOD_NAME compute
OUTPUT_FILE ${OUTPUT_TEST_DIR}/LinearSievePrimeCounterTests.cpp
STRONG_ASSERT 1
)

# <<< END test-generation


add_executable(AlgebraicAlgorithmsTests ${TEST_SOURCES}
    ${GENERATED_FILE}
    ${OUTPUT_TEST_DIR}/EuclideanGCDSubTests.cpp
    ${OUTPUT_TEST_DIR}/EuclidianGCDDivTests.cpp
    ${OUTPUT_TEST_DIR}/SteinGCDTests.cpp 
    ${OUTPUT_TEST_DIR}/IterativePowerCalculatorTests.cpp
    ${OUTPUT_TEST_DIR}/DoublingPowerCalculatorTests.cpp
    ${OUTPUT_TEST_DIR}/BinaryPowerCalculatorTests.cpp
    ${OUTPUT_TEST_DIR}/RecursiveFibCounterTests.cpp
    ${OUTPUT_TEST_DIR}/IterativeFibCounterTests.cpp
    ${OUTPUT_TEST_DIR}/GoldenRatioFibCounterTests.cpp
    ${OUTPUT_TEST_DIR}/MatrixFibCounterTests.cpp
    ${OUTPUT_TEST_DIR}/BruteForcePrimeCounterTests.cpp
    ${OUTPUT_TEST_DIR}/SqrtOptimizedPrimeCounterTests.cpp
    ${OUTPUT_TEST_DIR}/SievePrimeCounterTests.cpp
    ${OUTPUT_TEST_DIR}/BitSievePrimeCounterTests.cpp
    ${OUTPUT_TEST_DIR}/LinearSievePrimeCounterTests.cpp
    ../src/EuclideanGCDSub.cpp
    ../src/EuclidianGCDDiv.cpp
    ../src/SteinGCD.cpp
    ../src/StringToUint64Parser.cpp
    ../src/IterativePowerCalculator.cpp
    ../src/DoublingPowerCalculator.cpp
    ../src/BinaryPowerCalculator.cpp
    ../src/RecursiveFibCounter.cpp
    ../src/IterativeFibCounter.cpp
    ../src/GoldenRatioFibCounter.cpp
    ../src/MatrixFibCounter.cpp
    ../src/BruteForcePrimeCounter.cpp
    ../src/SqrtOptimizedPrimeCounter.cpp
    ../src/SievePrimeCounter.cpp
    ../src/BitSievePrimeCounter.cpp
    ../src/LinearSievePrimeCounter.cpp
)

target_include_directories(AlgebraicAlgorithmsTests PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(AlgebraicAlgorithmsTests gtest_main)

include(GoogleTest)
gtest_discover_tests(AlgebraicAlgorithmsTests)
