# ДЗ-15: Алгоритм Демукрона

Реализация алгоритма Демукрона для топологической сортировки с разбиением на уровни.

## Описание

Алгоритм Демукрона выполняет топологическую сортировку ориентированного ациклического 
графа (DAG) и разбивает вершины на уровни, где вершины одного уровня могут 
обрабатываться параллельно.

## Структура проекта

```
hw-15-demukron/
├── CMakeLists.txt
├── README.md
├── include/
│   ├── arrayslib/              # Библиотека структур данных из ДЗ-04
│   │   ├── Array.h
│   │   ├── FactorArray.h
│   │   ├── LinkedList.h
│   │   └── SpaceArray.h
│   └── DemukronAlgorithm.hpp   # Алгоритм Демукрона
├── demo/
│   └── main.cpp                # Демонстрационные примеры
├── tests/
│   └── DemukronTests.cpp       # Unit-тесты (GoogleTest)
└── docs/
    └── ALGORITHM_EXPLANATION.md # Подробное объяснение алгоритма
```

## Сборка

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

## Запуск

```bash
# Демонстрация
./demo

# Тесты
./tests
```

## Использование

```cpp
#include "DemukronAlgorithm.hpp"

// Создаём граф: 0 → 1 → 2
SpaceArray<int> adjList;
adjList.add(0, 1);  // ребро 0 → 1
adjList.add(1, 2);  // ребро 1 → 2

// Выполняем алгоритм
DemukronResult result = DemukronAlgorithm::execute(adjList, 3);

if (result.hasCycle) {
    // Граф содержит цикл
} else {
    // result.levels[k] содержит вершины уровня k
}
```

## Формат данных

**Входные данные**: Граф задан списком смежности через `SpaceArray<int>`, где 
`adjList[v]` содержит номера вершин, в которые ведут рёбра из v.

**Выходные данные**: `SpaceArray<int> levels`, где `levels[k]` содержит 
массив номеров вершин k-го уровня.

## Дополнительные задания

- [ ] Алгоритм Тарьяна (топологическая сортировка через DFS)
- [ ] Поиск мостов или точек сочленения