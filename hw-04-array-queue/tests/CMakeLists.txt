add_executable(arrays_tests
    all_test_runner.cpp
    DoublyLinkedListTests.cpp
    FactorArrayTest.cpp
    LinkedListTests.cpp
    MatrixArrayTest.cpp
    PriorityQueueArrayListTest.cpp
    SingleArrayTest.cpp
    SpaceArrayTest.cpp
    VectorArrayTest.cpp
)

target_link_libraries(arrays_tests PRIVATE
    arrayslib
    gtest
    gtest_main
)

add_test(NAME arrays_tests COMMAND arrays_tests)
