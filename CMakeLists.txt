cmake_minimum_required (VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project (Algo)

# GoogleTest setup
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

# Add subprojects
add_subdirectory(common)
#add_subdirectory ("Otus-Algorithm-2025-06-hw-01")
#add_subdirectory("hw-02-lucky-tickets")
#add_subdirectory("hw-03-algebraic-algorithms")
add_subdirectory("hw-04-array-queue")
#add_subdirectory("hw-05-bits")
#add_subdirectory("hw-06-search-and-sort")

add_subdirectory("hw-07-selectionsort-heapsort")

add_subdirectory("hw-08-quick-merge-external-sort")

add_subdirectory("hw-09-counting-radix-bucket-sort")

add_subdirectory("hw-10-bst-avl-treap")

add_subdirectory("hw-12-hash")

add_subdirectory("hw-13-trie")

add_subdirectory("hw-15-demucron")

add_subdirectory("hw-16-mst")

add_subdirectory("hw-17-shortest-paths")

add_subdirectory("hw-18-boyer-moore-search")

add_subdirectory("hw-19-kmp-search")

add_subdirectory("hw-21-dynamic-programming")

add_subdirectory("cache-lib")