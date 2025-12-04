cmake_minimum_required(VERSION 3.14)
project(hw-16-mst VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================
# Библиотека MST алгоритмов (header-only)
# ============================================
add_library(mst_algorithms INTERFACE)
target_include_directories(mst_algorithms INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(mst_algorithms INTERFACE arrayslib)

# ============================================
# Демонстрационное приложение
# ============================================
add_executable(hw16_demo demo/main.cpp)
target_link_libraries(hw16_demo mst_algorithms arrayslib)

# ============================================
# Тесты (GoogleTest)
# ============================================
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(hw16_tests 
    tests/UnionFindTest.cpp
    tests/KruskalMSTTest.cpp
    tests/PrimMSTTest.cpp
    tests/BoruvkaMSTTest.cpp
)

target_link_libraries(hw16_tests
    mst_algorithms
    arrayslib
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(hw16_tests)