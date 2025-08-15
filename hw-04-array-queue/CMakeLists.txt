project(ArrayQueue)

# Подключаем интерфейсы из папки include
include_directories(include)

# Основное приложение
add_executable(ArrayQueue
    "src/benchmark.cpp"
 "include/Array.h" "include/SingleArray.h"  "include/VectorArray.h" "include/FactorArray.h" "include/MatrixArray.h" "include/LinkedList.h" "include/DoublyLinkedList.h" "include/PriorityQueueArrayList.h")

# Добавляем тесты
add_subdirectory(tests)