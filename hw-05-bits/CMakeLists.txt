project(hw-05-bits)

cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Подключаем интерфейсы из папки include
include_directories(include)

set(CPP_SOURCES
    src/King.cpp
    src/Knight.cpp
)

set(TEST_SOURCES
    tests/all_test_runner.cpp
    tests/BitUtilsTest.cpp
    tests/KingTest.cpp
    tests/KnightTest.cpp
)

add_executable(MoveableTests ${CPP_SOURCES} ${TEST_SOURCES})


target_include_directories(MoveableTests PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(MoveableTests gtest_main)

include(GoogleTest)
gtest_discover_tests(MoveableTests)
