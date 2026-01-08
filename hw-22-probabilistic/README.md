# ДЗ-22: Вероятностные структуры данных

## Информация о задании

| Параметр | Значение |
|----------|----------|
| **Суммарный результат** | 15 байт |
| **Выполненные пункты** | Все |
| **Язык программирования** | C++17 |

## Структура проекта

```
hw-22-probabilistic/
├── include/sketch/
│   ├── IHasher.hpp            # Интерфейс хэш-сервиса
│   ├── StdHasher.hpp          # Реализация на std::hash
│   ├── MurmurHasher.hpp       # Реализация MurmurHash3
│   ├── HashUtils.hpp          # Утилиты для работы с битами
│   ├── BloomFilter.hpp        # Bloom Filter (header-only)
│   ├── HyperLogLog.hpp        # HyperLogLog (header-only)
│   └── CountMinSketch.hpp     # Count-Min Sketch (header-only)
├── tests/
│   └── SketchTests.cpp        # GoogleTest тесты
├── demo/
│   └── main.cpp               # Бенчмарки + сравнение хэшеров
├── CMakeLists.txt
├── LECTURE.md                 # Подробная лекция с примерами
└── README.md
```

## Реализованные алгоритмы

### 1. Bloom Filter — проверка принадлежности

**Гарантии:**
- `contains() = true` для добавленных элементов (нет False Negatives)
- `contains() = false` означает "точно нет"
- Возможны False Positives (с контролируемой вероятностью)

### 2. HyperLogLog — подсчёт уникальных элементов

**Гарантии:**
- Оценка cardinality с ошибкой σ ≈ 1.04 / √m
- Память O(log log n) — ~16 КБ для миллиардов элементов

### 3. Count-Min Sketch — подсчёт частоты

**Гарантии:**
- `estimate(x) >= actual_count(x)` — **никогда не занижает**
- Может завышать из-за коллизий
- С вероятностью ≥ 1-δ: estimate(x) ≤ actual(x) + ε × N

## Архитектура хэш-сервиса

Используется паттерн Strategy для подмены хэш-функций:

```cpp
// Интерфейс
class IHasher {
    virtual uint64_t hash(uint64_t value, uint64_t seed) = 0;
    virtual uint64_t hash(const std::string& value, uint64_t seed) = 0;
};

// Реализации
class StdHasher : public IHasher { ... };     // std::hash
class MurmurHasher : public IHasher { ... };  // MurmurHash3

// Использование
auto hasher = std::make_shared<MurmurHasher>();
BloomFilter bf(1000000, 0.01, hasher);
HyperLogLog hll(14, hasher);
CountMinSketch cms(0.01, 0.01, hasher);
```

## Сборка и запуск

```bash
# Из корня проекта
mkdir build && cd build
cmake ..
make

# Тесты
./hw-22-probabilistic/hw22_tests

# Бенчмарки
./hw-22-probabilistic/hw22_demo
```

## API

### Bloom Filter

```cpp
#include "sketch/BloomFilter.hpp"
#include "sketch/MurmurHasher.hpp"

auto hasher = std::make_shared<MurmurHasher>();
BloomFilter bf(1000000, 0.01, hasher);  // n=1M, FPR=1%

bf.add(uint64_t(12345));
bf.add(std::string("hello"));

if (!bf.contains(x)) {
    // x ТОЧНО нет в фильтре
}
```

### HyperLogLog

```cpp
#include "sketch/HyperLogLog.hpp"

HyperLogLog hll(14);  // p=14 → ошибка ~0.8%

for (auto& userId : users) {
    hll.add(userId);
}

uint64_t uniqueCount = hll.estimate();
```

### Count-Min Sketch

```cpp
#include "sketch/CountMinSketch.hpp"

CountMinSketch cms(0.01, 0.01);  // ε=1%, δ=1%

cms.add(std::string("video_A"));
cms.add(std::string("video_A"));

uint64_t count = cms.estimate(std::string("video_A"));  // >= 2
```

## Сравнение с точными решениями

| Структура | Точное решение | Экономия памяти | Ошибка |
|-----------|----------------|-----------------|--------|
| Bloom Filter | unordered_set | ~10x | FPR ~1% |
| HyperLogLog | unordered_set | ~1000x | ~1-2% |
| Count-Min Sketch | unordered_map | ~10-100x | Завышение |

## Тестовое покрытие

- Базовые операции
- Граничные условия
- Гарантии алгоритмов (нет FN для BF, не занижает для CMS)
- Сравнение хэшеров (std::hash vs MurmurHash3)
- Сравнение с точными структурами

## Результаты прогона бенчмарков

```bash
============================================
  Bloom Filter vs std::unordered_set
============================================

Параметры:
  n (элементов): 1000000
  Целевой FPR: 1%
  Методика: 5 прогонов по 100000 случайных запросов

Результаты:
| Метрика               | std::hash       | MurmurHash3     | unordered_set   |
|-----------------------|-----------------|-----------------|-----------------|
| Память (байт)         |         1198200 |         1198200 |        19577224 |
| FPR (факт)            |         1.0154% |         0.9998% |              0% |
| FPR (ожидание)        |         1.0000% |         1.0000% |              0% |

Экономия памяти: 16.3x (std::hash), 16.3x (MurmurHash3)

============================================
  HyperLogLog vs std::unordered_set
============================================

Параметры:
  Уникальных элементов: 1000000
  Всего добавлений: 5000000

Результаты:
| p  | Hasher      | Оценка      | Ошибка (факт) | Ошибка (σ)  | Память    |
|----|-------------|-------------|---------------|-------------|-----------|
| 10 |   std::hash |     1036509 |         4.36% |       3.25% |      1080 |
| 10 | MurmurHash3 |      991554 |         0.17% |       3.25% |      1080 |
| 12 |   std::hash |     1012091 |         1.90% |       1.62% |      4152 |
| 12 | MurmurHash3 |      990506 |         0.27% |       1.62% |      4152 |
| 14 |   std::hash |     1012940 |         1.99% |       0.81% |     16440 |
| 14 | MurmurHash3 |      990167 |         0.31% |       0.81% |     16440 |
| -- | exact       |      993215 |         0.00% |          -  |  19522944 |

Экономия памяти (p=14): 1188x
Ожидание: σ = 1.04 / sqrt(m), где m = 2^p

============================================
  Count-Min Sketch vs std::unordered_map
============================================

Параметры:
  Уникальных элементов: 10000
  Всего добавлений (N): 1000000
  Распределение: Zipf (экспонента = 1.0)

Результаты:
| ε       | Hasher      | Ср.ошибка | Макс.ошибка | Макс.допуст.(ε×N) | Память    |
|---------|-------------|-----------|-------------|-------------------|-----------|
|   0.010 |   std::hash |   1228.37 |        5206 |             10000 |     10944 |
|   0.010 | MurmurHash3 |   1237.59 |        5552 |             10000 |     10944 |
|   0.001 |   std::hash |     39.21 |         419 |              1000 |    108824 |
|   0.001 | MurmurHash3 |     39.06 |         443 |              1000 |    108824 |
| exact   | -           |      0.00 |           0 |                 - |    242184 |

Экономия памяти (ε=0.001): 2.2x
Гарантия: estimate(x) >= actual(x), ошибка <= ε×N с вероятностью >= 1-δ

============================================
  Демонстрация: CMS никогда не занижает
============================================

Добавляем: video_A — 100 раз, video_B — 50 раз, video_C — 0 раз

| Видео   | Реально | Оценка CMS | estimate >= actual? |
|---------|---------|------------|---------------------|
| video_A | 100     |        100 | ДА ✓                |
| video_B | 50      |         50 | ДА ✓                |
| video_C | 0       |          0 | ДА ✓                |
```