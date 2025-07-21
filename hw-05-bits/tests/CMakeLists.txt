set(TEST_SOURCES
    all_test_runner.cpp
    BitUtilsTest.cpp
    KingTest.cpp
    KnightTest.cpp
    RookTest.cpp 
    BishopTest.cpp 
    QueenTest.cpp
)

add_executable(MoveableTests ${TEST_SOURCES})

target_include_directories(MoveableTests PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(MoveableTests PRIVATE BitsLib gtest_main)

include(GoogleTest)
gtest_discover_tests(MoveableTests)
