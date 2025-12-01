cmake_minimum_required(VERSION 3.14)
project(hw-09-linear-sort VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Опции
option(BUILD_TESTS "Build unit tests" ON)
option(BUILD_DEMOS "Build demo applications" ON)

# Подключаем FetchContent для GoogleTest
include(FetchContent)

# Основная библиотека
add_library(linear_sorters
    src/CountingSorter.cpp
    src/RadixSorter.cpp
    src/BucketSorter.cpp
)

target_include_directories(linear_sorters PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(linear_sorters PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -O3>
)

# Demo applications
if(BUILD_DEMOS)
    add_executable(benchmark_junior demo/benchmark_junior.cpp)
    target_link_libraries(benchmark_junior PRIVATE linear_sorters commonlib)

    add_executable(benchmark_middle demo/benchmark_middle.cpp)
    target_link_libraries(benchmark_middle PRIVATE linear_sorters commonlib)

    add_executable(generate_file demo/generate_file.cpp)
endif()

# Tests
if(BUILD_TESTS)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    add_executable(test_linear_sorters tests/test_linear_sorters.cpp)
    target_link_libraries(test_linear_sorters PRIVATE
        linear_sorters
        GTest::gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(test_linear_sorters)
endif()