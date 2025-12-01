cmake_minimum_required(VERSION 3.14)
project(hw-10-bst-avl-treap VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Опции
option(BUILD_TESTS "Build unit tests" ON)
option(BUILD_DEMOS "Build demo applications" ON)

# Подключаем FetchContent для GoogleTest
include(FetchContent)

# Основная библиотека
add_library(binary_trees
    src/BinarySearchTree.cpp
    src/AVLTree.cpp
    src/Treap.cpp
)

target_include_directories(binary_trees PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(binary_trees PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -O2>
)

# Demo applications
if(BUILD_DEMOS)
    add_executable(benchmark_trees demo/benchmark_trees.cpp)
    target_link_libraries(benchmark_trees PRIVATE binary_trees commonlib)
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

    add_executable(test_trees tests/test_trees.cpp)
    target_link_libraries(test_trees PRIVATE
        binary_trees
        GTest::gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(test_trees)
endif()