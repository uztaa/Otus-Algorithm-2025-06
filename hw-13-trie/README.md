# HW-13: Trie (Prefix Tree) Implementation

## Содержание

1. [Обзор](#обзор)
2. [Уровни реализации](#уровни-реализации)
3. [Структура проекта](#структура-проекта)
4. [API](#api)
5. [Сборка и запуск](#сборка-и-запуск)
6. [Результаты тестирования](#результаты-тестирования)
7. [Результаты бенчмарков](#результаты-бенчмарков)
8. [Выводы](#выводы)

---

## Обзор

**Trie (префиксное дерево)** — древовидная структура данных для эффективного хранения и поиска строк. Каждый узел представляет один символ, а путь от корня формирует префикс или полное слово.

### Ключевые характеристики

| Операция | Сложность |
|----------|-----------|
| Insert | O(m) |
| Search | O(m) |
| StartsWith | O(m) |
| Remove | O(m) |
| GetWordsWithPrefix | O(m + k) |

*m — длина слова, k — количество результатов*

---

## Уровни реализации

### Junior Level (+5 байт) ✅

Базовый Trie с операциями:
- `insert(word)` — вставка слова
- `search(word)` — точный поиск
- `startsWith(prefix)` — поиск по префиксу

```cpp
Trie trie;
trie.insert("apple");
trie.search("apple");   // true
trie.search("app");     // false
trie.startsWith("app"); // true
trie.insert("app");
trie.search("app");     // true
```

### Middle Level (+5 байт) ✅

Ассоциативный массив `TrieMap<V>` на основе Trie:
- `put(key, value)` — добавление/обновление
- `get(key)` — получение значения
- `remove(key)` — удаление
- `getByPrefix(prefix)` — получение всех пар по префиксу
- `operator[]` — доступ по ключу

```cpp
TrieMap<int> phoneBook;
phoneBook.put("alice", 1001);
phoneBook.put("alex", 1002);

auto val = phoneBook.get("alice");     // 1001
auto entries = phoneBook.getByPrefix("al"); // alice, alex

phoneBook["bob"] = 1003; // operator[]
```

### Senior Level (+5 байт) ✅

Сравнительные бенчмарки Trie vs Hash Table:
- Insert performance
- Search performance
- Remove performance
- Prefix search performance
- Memory considerations

---

## Структура проекта

```
hw-13-trie/
├── include/
│   ├── ITrie.hpp         # Интерфейс Trie
│   ├── ITrieMap.hpp      # Интерфейс TrieMap
│   ├── TrieNode.hpp      # Структура узла
│   ├── Trie.hpp          # Объявление Trie
│   └── TrieMap.hpp       # Шаблонный TrieMap
├── src/
│   └── Trie.cpp              # Реализация Trie
├── tests/
│   ├── TrieTest.cpp          # Тесты Trie
│   └── TrieMapTest.cpp       # Тесты TrieMap
├── benchmarks/
│   └── Benchmark.cpp         # Сравнение с hash table
├── demo/
│   └── main.cpp              # Демонстрация
├── docs/
│   └── trie-lecture.md       # Конспект лекции
├── CMakeLists.txt
└── README.md
```

---

## API

### ITrie Interface

```cpp
class ITrie {
public:
    virtual void insert(const std::string& word) = 0;
    virtual bool search(const std::string& word) const = 0;
    virtual bool startsWith(const std::string& prefix) const = 0;
    virtual bool remove(const std::string& word) = 0;
    virtual std::vector<std::string> getWordsWithPrefix(const std::string& prefix) const = 0;
    virtual size_t size() const = 0;
    virtual bool empty() const = 0;
    virtual void clear() = 0;
};
```

### ITrieMap Interface

```cpp
template<typename V>
class ITrieMap {
public:
    virtual void put(const std::string& key, const V& value) = 0;
    virtual std::optional<V> get(const std::string& key) const = 0;
    virtual bool remove(const std::string& key) = 0;
    virtual bool contains(const std::string& key) const = 0;
    virtual std::vector<std::pair<std::string, V>> getByPrefix(const std::string& prefix) const = 0;
    virtual std::vector<std::string> keysWithPrefix(const std::string& prefix) const = 0;
    virtual std::vector<std::string> keys() const = 0;
    virtual size_t size() const = 0;
    virtual bool empty() const = 0;
    virtual void clear() = 0;
};
```

---

## Сборка и запуск

### Требования

- C++17 compatible compiler (GCC 8+, Clang 7+)
- CMake 3.16+
- GoogleTest (опционально, для тестов)

### Сборка

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j4
```

### Запуск

```bash
# Демонстрация
./trie_demo

# Тесты
./trie_test
./triemap_test

# Бенчмарки
./trie_benchmark
```

---

## Результаты тестирования

### Trie Tests

| Категория | Тесты | Статус |
|-----------|-------|--------|
| Insert | 4 теста | ✅ |
| Search | 5 тестов | ✅ |
| StartsWith | 4 теста | ✅ |
| Remove | 5 тестов | ✅ |
| GetWordsWithPrefix | 4 теста | ✅ |
| Size/Empty | 3 теста | ✅ |
| Clear | 2 теста | ✅ |
| Edge Cases | 4 теста | ✅ |
| **LeetCode 208** | 1 тест | ✅ |

### TrieMap Tests

| Категория | Тесты | Статус |
|-----------|-------|--------|
| Put/Get | 5 тестов | ✅ |
| Contains | 3 теста | ✅ |
| Remove | 4 теста | ✅ |
| GetByPrefix | 3 теста | ✅ |
| KeysWithPrefix | 1 тест | ✅ |
| Keys | 2 теста | ✅ |
| operator[] | 3 теста | ✅ |
| Edge Cases | 3 теста | ✅ |

---

## Результаты бенчмарков

> **Окружение:** 100,000 случайных строк длиной 5-15 символов

### Point Operations (Trie vs HashTable)

| Операция | Trie | HashSet/Map | Победитель |
|:---------|-----:|------------:|:----------:|
| **Insert** | 3.03×10⁵ ops/s | 2.88×10⁶ ops/s | 🏆 Hash ×9.5 |
| **Search** | 4.84×10⁵ ops/s | 1.64×10⁶ ops/s | 🏆 Hash ×3.4 |
| **Remove** | 1.61×10⁵ ops/s | 6.82×10⁶ ops/s | 🏆 Hash ×42 |
| **MapPut** | 9.95×10⁵ ops/s | 6.24×10⁶ ops/s | 🏆 Hash ×6.3 |
| **MapGet** | 9.58×10⁵ ops/s | 3.57×10⁶ ops/s | 🏆 Hash ×3.7 |

### Prefix Operations (Trie vs HashTable)

| Операция | Trie | HashSet | Победитель |
|:---------|-----:|--------:|:----------:|
| **PrefixSearch** (10K запросов) | 3.17×10³ ops/s | 2.20×10² ops/s | 🏆 **Trie ×14.4** |
| **Autocomplete** (~10K запросов) | 1.21×10³ ops/s | 2.27×10² ops/s | 🏆 **Trie ×5.3** |

> *PrefixSearch: в среднем 148 результатов на запрос*

### Сводная таблица

```
┌─────────────────┬────────────────────────────────────────┐
│   Operation     │              Winner                    │
├─────────────────┼────────────────────────────────────────┤
│ Insert          │ HashTable ████████████████████  ×9.5   │
│ Search          │ HashTable ██████                ×3.4   │
│ Remove          │ HashTable ████████████████████████ ×42 │
│ MapPut          │ HashTable ████████████          ×6.3   │
│ MapGet          │ HashTable ██████                ×3.7   │
├─────────────────┼────────────────────────────────────────┤
│ PrefixSearch    │ Trie ██████████████████████████ ×14.4  │
│ Autocomplete    │ Trie ██████████                 ×5.3   │
└─────────────────┴────────────────────────────────────────┘
```

### Когда использовать Trie

| ✅ Trie лучше | ❌ Hash Table лучше |
|--------------|---------------------|
| Поиск по префиксу (autocomplete) | Только точный поиск |
| Проверка орфографии | Случайные строки без паттернов |
| IP-маршрутизация | Критичная скорость Insert/Remove |
| Longest prefix matching | Простота реализации |
| Упорядоченный обход ключей | Минимум памяти на случайных данных |

---

## Демонстрация работы

```
BASIC TRIE (Junior Level)
--------------------------
trie.insert("apple")
trie.search("apple")   -> true
trie.search("app")     -> false
trie.startsWith("app") -> true
trie.insert("app")
trie.search("app")     -> true

AUTOCOMPLETE DEMO
-----------------
User types: "c"
Suggestions: car, card, care, careful, careless ... (12 more)

User types: "car"
Suggestions: car, card, care, careful, careless

User types: "cod"
Suggestions: code, coder, coding
```

---

## Выводы

### Реализация

1. **Junior Level**: Полностью реализован базовый Trie согласно спецификации LeetCode 208
2. **Middle Level**: Реализован шаблонный `TrieMap<V>` с поддержкой всех операций ассоциативного массива
3. **Senior Level**: Проведены бенчмарки сравнения с `unordered_set` и `unordered_map`

### Архитектурные решения

- Использование интерфейсов `ITrie`, `ITrieMap`
- Узел реализован через `std::array<unique_ptr, 26>` для O(1) доступа
- Шаблонный `TrieMap` для поддержки произвольных типов значений
- RAII через `unique_ptr` для автоматического управления памятью

### Ключевые наблюдения

| Аспект | Вывод |
|--------|-------|
| **Point ops** | Hash Table быстрее в 3-42× (O(1) avg vs O(m)) |
| **Prefix ops** | Trie быстрее в 5-14× (O(p+k) vs O(n×p)) |
| **Память** | Trie экономит на общих префиксах |
| **Применение** | Autocomplete, spell-check, IP routing |