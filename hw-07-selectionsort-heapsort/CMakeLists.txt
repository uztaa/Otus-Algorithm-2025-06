project(hw-07-selection-heap-sort)

# Compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# Find common library (assuming it's in a sibling directory)
set(COMMON_DIR "${CMAKE_SOURCE_DIR}/common" CACHE PATH "Path to common library")

if(NOT EXISTS ${COMMON_DIR})
    message(WARNING "Common library not found at ${COMMON_DIR}. Please set COMMON_DIR correctly.")
endif()

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/hw-07-selectionsort-heapsort/include
    ${COMMON_DIR}/include
)

# Common library sources
set(COMMON_SOURCES
    ${COMMON_DIR}/src/BaseSorter.cpp
    ${COMMON_DIR}/src/LoggingSortListener.cpp
    ${COMMON_DIR}/src/PrintUtils.cpp
    ${COMMON_DIR}/src/RandomArrayGenerator.cpp
    ${COMMON_DIR}/src/SortEventFormatter.cpp
    ${COMMON_DIR}/src/TimeFormatter.cpp
    ${COMMON_DIR}/src/Timer.cpp
)

# hw-07 library
add_library(hw07lib
    src/SelectionSorter.cpp
    src/HeapSorter.cpp
    ${COMMON_SOURCES}
)

# Demo executables - ИЗМЕНИТЬ ТОЛЬКО ИМЕНА TARGETS, НЕ ФАЙЛОВ
add_executable(hw07_demo_selection_heap_sort
    demo/demo_selection_heap_sort.cpp
)
target_link_libraries(hw07_demo_selection_heap_sort hw07lib)

add_executable(hw07_benchmark_sorters
    demo/benchmark_sorters.cpp
)
target_link_libraries(hw07_benchmark_sorters hw07lib)

add_executable(hw07_benchmark_sorters_extended
    demo/benchmark_sorters_extended.cpp
)
target_link_libraries(hw07_benchmark_sorters_extended hw07lib)

# Testing
option(BUILD_TESTS "Build tests" ON)

if(BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    add_executable(hw07_test_sort_all
        tests/test_sort_all.cpp
    )
    target_link_libraries(hw07_test_sort_all 
        hw07lib
        GTest::gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(hw07_test_sort_all)
endif()

# Installation
install(TARGETS hw07_demo_selection_heap_sort hw07_benchmark_sorters hw07_benchmark_sorters_extended
    DESTINATION bin
)