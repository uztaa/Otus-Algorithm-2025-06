cmake_minimum_required(VERSION 3.16)
project(hw-13-trie LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(trie_lib src/Trie.cpp)

add_executable(trie_demo demo/main.cpp)
target_link_libraries(trie_demo PRIVATE trie_lib)

add_executable(trie_benchmark benchmarks/Benchmark.cpp)
target_link_libraries(trie_benchmark PRIVATE trie_lib)

enable_testing()
find_package(GTest QUIET)

if(NOT GTest_FOUND)
    include(FetchContent)
    FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    FetchContent_MakeAvailable(googletest)
endif()

add_executable(trie_test tests/TrieTest.cpp)
target_link_libraries(trie_test PRIVATE trie_lib GTest::gtest GTest::gtest_main)
add_test(NAME TrieTest COMMAND trie_test)

add_executable(triemap_test tests/TrieMapTest.cpp)
target_link_libraries(triemap_test PRIVATE trie_lib GTest::gtest GTest::gtest_main)
add_test(NAME TrieMapTest COMMAND triemap_test)