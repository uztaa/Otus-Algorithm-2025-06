project(hw-19-kmp-search)

# ============================================
# Библиотека алгоритмов поиска подстроки
# ============================================
add_library(kmplib
    src/AutomatonSearch.cpp
    src/KMPSearchSlow.cpp
    src/KMPSearch.cpp
)

target_include_directories(kmplib 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ============================================
# Демонстрационное приложение с бенчмарками
# ============================================
add_executable(hw19_demo demo/main.cpp)

target_link_libraries(hw19_demo 
    PRIVATE 
        kmplib
)

# ============================================
# Тесты
# ============================================
add_executable(hw19_tests tests/SearchTests.cpp)

target_link_libraries(hw19_tests
    PRIVATE
        kmplib
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(hw19_tests)
