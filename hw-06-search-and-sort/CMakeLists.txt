cmake_minimum_required(VERSION 3.15)
project(sorting_search_lib)


# Подпроект библиотеки
add_library(sorting_search_lib
    src/sort/BubbleSorter.cpp
    src/search/SequentialSearcher.cpp
 "src/search/MoveToFrontSearcher.cpp" "src/search/TranspositionSearcher.cpp" "src/sort/InsertionSorter.cpp" "src/sort/InsertionShiftSorter.cpp" "src/sort/BinaryInsertionSorter.cpp" "src/sort/CocktailSorter.cpp" "src/search/IndexSearcher.cpp" "src/sort/CustomShellSorter.cpp")

target_include_directories(sorting_search_lib PUBLIC include)

# Установка флагов компиляции
add_executable(benchmark_sort tests/benchmark/benchmark_sort_parallel.cpp)
# Линкуем с библиотекой сортировок
target_link_libraries(benchmark_sort sorting_search_lib)



# Unit-tests
set(TEST_INCLUDE_FILES tests/record_test.cpp "tests/test_index_searcher.cpp" "tests/test_binary_insertion_sorter.cpp" "tests/test_binary_insertion_sorter_binary_search.cpp")
add_executable(unit_tests ${TEST_INCLUDE_FILES})

target_link_libraries(unit_tests sorting_search_lib gtest gtest_main)

