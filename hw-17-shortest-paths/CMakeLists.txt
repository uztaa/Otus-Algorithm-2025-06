project(hw-17-shortest-paths VERSION 1.0.0 LANGUAGES CXX)

# ============================================
# Библиотека алгоритмов кратчайших путей (header-only)
# ============================================
add_library(shortest_paths INTERFACE)
target_include_directories(shortest_paths INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(shortest_paths INTERFACE arrayslib)

# ============================================
# Демонстрационное приложение
# ============================================
add_executable(hw17_demo demo/main.cpp)
target_link_libraries(hw17_demo shortest_paths arrayslib)

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

add_executable(hw17_tests
    tests/DijkstraTest.cpp
    tests/FloydWarshallTest.cpp
)

target_link_libraries(hw17_tests
    shortest_paths
    arrayslib
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(hw17_tests)