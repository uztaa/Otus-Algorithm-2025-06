project(hw-21-dp CXX)

# Директории с исходными файлами
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Исходные файлы для библиотеки DP
set(DP_SOURCES
    src/Task1_Fractions.cpp
    src/Task2_Garland.cpp
    src/Task3_FiveEight.cpp
    src/Task4_Islands.cpp
    src/Task5_SmallBarn.cpp
    src/Task6_BarnHeight.cpp
    src/Task7_BarnWidth.cpp
    src/Task8_LargeBarn.cpp
)

# Создание статической библиотеки
add_library(dp_lib STATIC ${DP_SOURCES})
target_include_directories(dp_lib PUBLIC include)

# Демонстрационное приложение
add_executable(demo_dp demo/main.cpp)
target_link_libraries(demo_dp PRIVATE dp_lib)

# Опции компилятора
if(MSVC)
    target_compile_options(demo_dp PRIVATE /W4)
else()
    target_compile_options(demo_dp PRIVATE -Wall -Wextra -Wpedantic)
endif()


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

add_executable(hw21_tests 
    tests/test_task1.cpp
    tests/test_task2.cpp
    tests/test_task3.cpp
    tests/test_task4.cpp
    tests/test_task5.cpp
    tests/test_task6.cpp
    tests/test_task7.cpp
    tests/test_task8.cpp
)

target_link_libraries(hw21_tests
    dp_lib
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(hw21_tests)