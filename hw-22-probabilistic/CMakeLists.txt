project(hw-22-probabilistic)

# ============================================
# Header-only библиотека вероятностных структур данных
# ============================================
add_library(sketchlib INTERFACE)

target_include_directories(sketchlib 
    INTERFACE 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ============================================
# Демонстрационное приложение с бенчмарками
# ============================================
add_executable(hw22_demo demo/main.cpp)

target_link_libraries(hw22_demo 
    PRIVATE 
        sketchlib
)

# ============================================
# Тесты
# ============================================
add_executable(hw22_tests tests/SketchTests.cpp)

target_link_libraries(hw22_tests
    PRIVATE
        sketchlib
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(hw22_tests)
