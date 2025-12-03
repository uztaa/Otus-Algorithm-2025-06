cmake_minimum_required(VERSION 3.15)

project(hw04_array_queue LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --------------------------
# Header-only Library
# --------------------------
add_library(arrayslib INTERFACE)

target_include_directories(arrayslib INTERFACE include)

# --------------------------
# Benchmark
# --------------------------
add_executable(arrays_benchmark src/benchmark.cpp)
target_link_libraries(arrays_benchmark PRIVATE arrayslib)

# --------------------------
# Tests
# --------------------------
add_subdirectory(tests)
