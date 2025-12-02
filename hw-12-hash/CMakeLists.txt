cmake_minimum_required(VERSION 3.14)
project(hw-12-hash VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Опции
option(BUILD_TESTS "Build unit tests" ON)
option(BUILD_DEMOS "Build demo applications" ON)

# Подключаем FetchContent для GoogleTest
include(FetchContent)

# Основная библиотека
add_library(hash_tables
    src/Common.cpp
)

target_include_directories(hash_tables PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(hash_tables PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic -O2>
)

# Demo applications
if(BUILD_DEMOS)
    add_executable(demo_chaining demo/demo_chaining.cpp)
    target_link_libraries(demo_chaining PRIVATE hash_tables)
    
    add_executable(demo_open_addressing demo/demo_open_addressing.cpp)
    target_link_libraries(demo_open_addressing PRIVATE hash_tables)
    
    add_executable(benchmark demo/benchmark.cpp)
    target_link_libraries(benchmark PRIVATE hash_tables)
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
    
    add_executable(test_common tests/test_common.cpp)
    target_link_libraries(test_common PRIVATE
        hash_tables
        GTest::gtest_main
    )
    
    add_executable(test_chaining tests/test_chaining_hash_table.cpp)
    target_link_libraries(test_chaining PRIVATE
        hash_tables
        GTest::gtest_main
    )
    
    add_executable(test_open_addressing tests/test_open_addressing_hash_table.cpp)
    target_link_libraries(test_open_addressing PRIVATE
        hash_tables
        GTest::gtest_main
    )
    
    include(GoogleTest)
    gtest_discover_tests(test_common)
    gtest_discover_tests(test_chaining)
    gtest_discover_tests(test_open_addressing)
endif()