cmake_minimum_required(VERSION 3.21)
project(sorting_search_lib)

set(CMAKE_CXX_STANDARD 20)

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

# Benchmark
add_executable(benchmark_sort tests/benchmark/benchmark_sort_parallel.cpp)
target_link_libraries(benchmark_sort sorting_search_lib)

# Unit-tests
add_executable(unit_tests
    tests/record_test.cpp
    tests/test_index_searcher.cpp
    tests/test_binary_insertion_sorter_binary_search.cpp
    tests/test_sort_all.cpp
    tests/test_custom_shell_sorter.cpp
)
target_link_libraries(unit_tests sorting_search_lib gtest gtest_main)


add_executable(main src/main.cpp)

set(vcpkg_installed "C:/Users/toban/source/repos/vcpkg/installed/x64-windows")
set(SFML_DIR "${vcpkg_installed}/share/sfml")
set(imgui_DIR "${vcpkg_installed}/share/imgui")
set(ImGui-SFML_DIR "${vcpkg_installed}/share/imgui-sfml")

# Подключение SFML
find_package(SFML 3.0.1 COMPONENTS System Window Graphics Audio Network CONFIG REQUIRED)
target_link_libraries(main PRIVATE SFML::Network SFML::Graphics SFML::Window SFML::Audio SFML::System)

# VCPKG подключит imgui через IMGUI-SFML
find_package(ImGui-SFML CONFIG REQUIRED)
target_link_libraries(main PRIVATE ImGui-SFML::ImGui-SFML)



if (MSVC)
    add_custom_command(TARGET main POST_BUILD
        # Копируем ImGui-SFML dll
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:ImGui-SFML::ImGui-SFML> "$<TARGET_FILE_DIR:main>/ImGui-SFML_d.dll"

        # Копируем SFML dll
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SFML::Graphics> "$<TARGET_FILE_DIR:main>/sfml-graphics-d.dll"

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SFML::Window> "$<TARGET_FILE_DIR:main>/sfml-window-d.dll"

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SFML::System> "$<TARGET_FILE_DIR:main>/sfml-system-d.dll"

        # Копируем freetype.dll как freetyped.dll
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${vcpkg_installed}/bin/freetype.dll" "$<TARGET_FILE_DIR:main>/freetyped.dll"

        # Трюк с подменой релизных DLL в Debug версии
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${vcpkg_installed}/debug/bin/sfml-graphics-d-3.dll" "$<TARGET_FILE_DIR:main>/sfml-graphics-d.dll"

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${vcpkg_installed}/debug/bin/sfml-window-d-3.dll" "$<TARGET_FILE_DIR:main>/sfml-window-d.dll"

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${vcpkg_installed}/debug/bin/sfml-system-d-3.dll" "$<TARGET_FILE_DIR:main>/sfml-system-d-3.dll"

        # Копируем zlib1.dll
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${vcpkg_installed}/bin/zlib1.dll" "$<TARGET_FILE_DIR:main>/zlib1.dll"

            # Копируем bz2.dll
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${vcpkg_installed}/bin/bz2.dll" "$<TARGET_FILE_DIR:main>/bz2.dll"

            # Копируем libpng16.dll
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${vcpkg_installed}/bin/libpng16.dll" "$<TARGET_FILE_DIR:main>/libpng16.dll"

            # brotlidec.dll
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${vcpkg_installed}/bin/brotlidec.dll" "$<TARGET_FILE_DIR:main>/brotlidec.dll"

            #brotlicommon.dll
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${vcpkg_installed}/bin/brotlicommon.dll" "$<TARGET_FILE_DIR:main>/brotlicommon.dll"
    )
endif()


  