project(hw-20-compression)

# ============================================
# Библиотека алгоритмов сжатия
# ============================================
add_library(compressionlib
    src/RLECompressor.cpp
    src/PackBitsCompressor.cpp
)

target_include_directories(compressionlib 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ============================================
# CLI-утилита и бенчмарки
# ============================================
add_executable(hw20_demo demo/main.cpp)

target_link_libraries(hw20_demo 
    PRIVATE 
        compressionlib
)

# ============================================
# Тесты
# ============================================
add_executable(hw20_tests tests/CompressionTests.cpp)

target_link_libraries(hw20_tests
    PRIVATE
        compressionlib
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(hw20_tests)
