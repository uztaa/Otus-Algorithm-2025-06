# ArraysLib — коллекции и структуры данных на C++

Библиотека содержит набор учебных реализаций динамических массивов, списков и связанных структур данных.  
Все структуры реализованы в виде отдельных классов и покрыты тестами GoogleTest.

## 📦 Возможности

### Реализованные структуры:

| Структура                   | Описание |
|-----------------------------|----------|
| `Array<T>`                  | Базовый интерфейс динамического массива |
| `SingleArray<T>`            | Масштабирование +1 элемент при каждом расширении |
| `VectorArray<T>`            | Масштабирование фиксированным шагом (10) |
| `FactorArray<T>`            | Масштабирование в фактор (`*2`) |
| `MatrixArray<T>`            | Массив блоков фиксированного размера |
| `LinkedList<T>`             | Односвязный список |
| `DoublyLinkedList<T>`       | Двусвязный список |
| `PriorityQueueArrayList<T>` | Очередь с приоритетом на базе списка списков |
| `SpaceArray<T>`             | Структура для разреженных данных (массив списков) |

## 📚 Использование в проекте

```cmake
add_subdirectory(hw-04)
target_link_libraries(my_app PRIVATE arrayslib)
````

В C++:

```cpp
#include "VectorArray.h"

int main() {
    VectorArray<int> arr;
    arr.add(10, 0);
    arr.add(20, 1);
    return arr.get(0);
}
```

## 🧪 Тестирование

Проект использует GoogleTest.

Сборка тестов:

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build
```

## 🏎 Benchmark

Исполняемый файл:

```bash
./benchmark
```

Генерирует `benchmark_report.html` с таблицей сравнения производительности для `add()`, `get()`, `remove()`.
