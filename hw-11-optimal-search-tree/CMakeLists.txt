project(hw-11-optimal-search-tree)

# Основная библиотека ДОП (header-only)
add_library(optimallib INTERFACE)
target_include_directories(optimallib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(optimallib INTERFACE binary_trees)

# Бенчмарк
add_executable(hw11_benchmark demo/benchmark.cpp)
target_link_libraries(hw11_benchmark PRIVATE optimallib binary_trees)

# ============================================
# Тесты
# ============================================
add_executable(hw11_tests tests/OptimalTreeTests.cpp)

target_link_libraries(hw11_tests
    PRIVATE
        optimallib
        binary_trees
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(hw11_tests)
