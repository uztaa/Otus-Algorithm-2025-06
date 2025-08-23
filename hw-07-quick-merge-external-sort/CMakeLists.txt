cmake_minimum_required(VERSION 3.14)
project(hw-07-quick-merge-external-sort VERSION 1.0.0 LANGUAGES CXX)

# Установка стандарта C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Сбор исходников (только cpp)
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Создание библиотеки
add_library(sorters_lib ${SOURCES})
target_include_directories(sorters_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# ================= GoogleTest =================
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

enable_testing()
file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")

add_executable(run_tests ${TEST_SOURCES})
target_link_libraries(run_tests
    sorters_lib
    GTest::gtest_main
    GTest::gmock_main
)

include(GoogleTest)
gtest_discover_tests(run_tests)

# ================= Demo =================
add_executable(demo_quick_merge_sort demo/demo_quick_merge_sort.cpp)
target_link_libraries(demo_quick_merge_sort sorters_lib)

# ================= Benchmark (JUNIOR) =================
add_executable(benchmark_sorters demo/benchmark_sorters.cpp)
target_link_libraries(benchmark_sorters sorters_lib)

# ================= Benchmark (MIDDLE) =================
add_executable(benchmark_external_sorters demo/benchmark_external_sorters.cpp)
target_link_libraries(benchmark_external_sorters sorters_lib)

# ================= Benchmark (SENIOR) =================
add_executable(benchmark_sorters_extended demo/benchmark_sorters_extended.cpp)
target_link_libraries(benchmark_sorters_extended sorters_lib)

# ================= Info =================
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD}")
