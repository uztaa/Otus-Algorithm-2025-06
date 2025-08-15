project(AlgebraicAlgorithms)

# Подключаем интерфейсы из папки include
include_directories(include)

# Основное приложение
add_executable(AlgebraicAlgorithms
    src/main.cpp
    "include/EuclidianGCDDiv.h" "src/EuclidianGCDDiv.cpp" "include/SteinGCD.h" "src/SteinGCD.cpp" "include/StringToUint64Parser.h" "src/StringToUint64Parser.cpp"  "include/PowerCalculator.h" "src/IterativePowerCalculator.cpp" "include/IterativePowerCalculator.h" "include/MathSettings.h" "include/DoublingPowerCalculator.h" "src/DoublingPowerCalculator.cpp" "include/BinaryPowerCalculator.h" "src/BinaryPowerCalculator.cpp" "include/Counter.h" "include/RecursiveFibCounter.h" "src/RecursiveFibCounter.cpp" "include/IterativeFibCounter.h" "src/IterativeFibCounter.cpp" "include/GoldenRatioFibCounter.h" "src/GoldenRatioFibCounter.cpp" "include/MatrixFibCounter.h" "src/MatrixFibCounter.cpp" "include/BruteForcePrimeCounter.h" "src/BruteForcePrimeCounter.cpp"  "include/SqrtOptimizedPrimeCounter.h" "src/SqrtOptimizedPrimeCounter.cpp" "include/SievePrimeCounter.h" "src/SievePrimeCounter.cpp" "include/BitSievePrimeCounter.h" "src/BitSievePrimeCounter.cpp" "include/LinearSievePrimeCounter.h" "src/LinearSievePrimeCounter.cpp")

# Добавляем тесты
add_subdirectory(tests)