cmake_minimum_required(VERSION 3.21)
project(sorting_search_lib)

set(CMAKE_CXX_STANDARD 20)

# === Исходники библиотеки сортировки ===
include_directories(include)

add_library(sorting_search_lib
    src/sort/BubbleSorter.cpp
    src/search/SequentialSearcher.cpp
    src/sort/BaseSorter.cpp
    src/search/MoveToFrontSearcher.cpp
    src/search/TranspositionSearcher.cpp
    src/sort/InsertionSorter.cpp
    src/sort/InsertionShiftSorter.cpp
    src/sort/BinaryInsertionSorter.cpp
    src/sort/CocktailSorter.cpp
    src/search/IndexSearcher.cpp
    src/sort/CustomShellSorter.cpp
    src/sort/LoggingSortListener.cpp
    src/sort/ProgressBarListener.cpp
)

target_include_directories(sorting_search_lib PUBLIC include)

# === Benchmark ===
add_executable(benchmark_sort tests/benchmark/benchmark_sort_parallel.cpp)
target_link_libraries(benchmark_sort sorting_search_lib)

# === Unit-tests ===
add_executable(unit_tests
    tests/record_test.cpp
    tests/test_index_searcher.cpp
    tests/test_binary_insertion_sorter_binary_search.cpp
    tests/test_sort_all.cpp
    tests/test_custom_shell_sorter.cpp
)
target_link_libraries(unit_tests sorting_search_lib gtest gtest_main)

# === Подключение SFML через vcpkg ===
set(vcpkg_installed "C:/Users/toban/source/repos/vcpkg/installed/x64-windows")
set(SFML_DIR "${vcpkg_installed}/share/sfml")

find_package(SFML 3 COMPONENTS System Window Graphics Audio Network CONFIG REQUIRED)

# === Подключение ImGui и ImGui-SFML из исходников ===
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui)
set(IMGUI_SFML_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui-sfml)
set(IMGUI_SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${IMGUI_SFML_DIR})

# === Основной GUI-экзешник ===
add_executable(main "src/main.cpp")

target_include_directories(main PRIVATE
    ${IMGUI_DIR}
    ${IMGUI_SFML_DIR}
)

target_link_libraries(main PRIVATE
    sorting_search_lib
    ImGui-SFML
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Audio
    SFML::Network
)
