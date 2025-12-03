project(hw-15-demukron VERSION 1.0.0 LANGUAGES CXX)

# ============================================
# Библиотека алгоритмов графов (header-only)
# ============================================
add_library(graph_algorithms INTERFACE)
target_include_directories(graph_algorithms INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(graph_algorithms INTERFACE arrayslib)

# ============================================
# Демонстрационное приложение
# ============================================
add_executable(demo demo/main.cpp)
target_link_libraries(demo graph_algorithms arrayslib)

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

add_executable(tests 
    tests/BridgesAPTests.cpp
    tests/DemukronTests.cpp
    tests/TarjanSCCTests.cpp
)
target_link_libraries(tests
    graph_algorithms
    arrayslib
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)
