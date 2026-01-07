project(hw-18-boyer-moore-search)

# Библиотека
add_library(searchlib
    src/NaiveSearch.cpp
    src/BMHSearch.cpp
    src/BoyerMooreSearch.cpp
)
target_include_directories(searchlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Демо
add_executable(hw18_demo demo/main.cpp)
target_link_libraries(hw18_demo PRIVATE searchlib commonlib)

# Тесты
add_executable(hw18_tests tests/SearchTests.cpp)
target_link_libraries(hw18_tests PRIVATE searchlib GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(hw18_tests)