# Префиксное дерево (Trie)

---

## 1. Введение

### 1.1 Определение

**Trie** (от слова "retrieval", произносится как "try") — это древовидная структура данных для эффективного хранения и поиска строк. Каждый узел представляет один символ, а путь от корня к узлу формирует префикс или полное слово.

### 1.2 Исторический контекст

Структура была предложена Эдвардом Фредкином в 1960 году. Название происходит от слова "re**trie**val" (извлечение), хотя сейчас произносится как "try" для отличия от "tree".

### 1.3 Применение

| Область | Пример использования |
|---------|---------------------|
| Автодополнение | Поисковые системы, IDE |
| Проверка орфографии | Текстовые редакторы |
| IP-маршрутизация | Longest prefix matching |
| Словари | T9, предиктивный ввод |
| Биоинформатика | Поиск по геномным данным |
| Компиляторы | Таблицы символов |

---

## 2. Структура данных

### 2.1 Базовая структура узла

```
          root (пустой)
         /    \
        a      b
       /        \
      p          e
     / \          \
    p   r         a
   /     \         \
  l       i        r  [end]
 /         \
e [end]    l [end]
```

Слова в дереве: "apple", "april", "bear"

### 2.2 Компоненты узла

```cpp
struct TrieNode {
    std::array<TrieNode*, 26> children;  // для lowercase English letters
    bool isEndOfWord;                     // маркер конца слова
    
    TrieNode() : children{}, isEndOfWord(false) {}
};
```

### 2.3 Альтернативные реализации узла

**1. Массив фиксированного размера (26 символов)**
- Плюсы: O(1) доступ к потомку
- Минусы: O(26) память на узел, даже если большинство пустые

**2. Хэш-таблица**
```cpp
std::unordered_map<char, TrieNode*> children;
```
- Плюсы: экономия памяти для разреженных данных
- Минусы: накладные расходы хэширования

**3. Отсортированный массив**
```cpp
std::vector<std::pair<char, TrieNode*>> children;  // отсортирован по char
```
- Плюсы: компактность + упорядоченный обход
- Минусы: O(log k) поиск через бинарный поиск

---

## 3. Основные операции

### 3.1 Вставка (Insert)

**Алгоритм:**
1. Начать с корня
2. Для каждого символа слова:
   - Если потомок с этим символом не существует — создать
   - Перейти к потомку
3. Отметить последний узел как конец слова

**Сложность:** O(m), где m — длина слова

```cpp
void insert(const std::string& word) {
    TrieNode* current = root;
    for (char c : word) {
        int index = c - 'a';
        if (!current->children[index]) {
            current->children[index] = new TrieNode();
        }
        current = current->children[index];
    }
    current->isEndOfWord = true;
}
```

### 3.2 Поиск (Search)

**Алгоритм:**
1. Начать с корня
2. Для каждого символа слова:
   - Если потомок не существует — вернуть false
   - Перейти к потомку
3. Вернуть значение isEndOfWord последнего узла

**Сложность:** O(m)

```cpp
bool search(const std::string& word) {
    TrieNode* node = findNode(word);
    return node != nullptr && node->isEndOfWord;
}
```

### 3.3 Поиск по префиксу (StartsWith)

**Алгоритм:**
1. Аналогично search, но без проверки isEndOfWord
2. Достаточно дойти до конца префикса

**Сложность:** O(m)

```cpp
bool startsWith(const std::string& prefix) {
    return findNode(prefix) != nullptr;
}
```

### 3.4 Удаление (Delete)

**Алгоритм (рекурсивный):**
1. Найти узел, соответствующий слову
2. Снять флаг isEndOfWord
3. Если узел не имеет потомков — удалить его
4. Рекурсивно удалять родительские узлы, если они стали "пустыми"

**Сложность:** O(m)

```cpp
bool remove(const std::string& word) {
    return removeHelper(root, word, 0);
}

bool removeHelper(TrieNode* node, const std::string& word, size_t depth) {
    if (!node) return false;
    
    if (depth == word.size()) {
        if (!node->isEndOfWord) return false;
        node->isEndOfWord = false;
        return isEmpty(node);  // можно ли удалить этот узел?
    }
    
    int index = word[depth] - 'a';
    if (removeHelper(node->children[index], word, depth + 1)) {
        delete node->children[index];
        node->children[index] = nullptr;
        return !node->isEndOfWord && isEmpty(node);
    }
    return false;
}
```

---

## 4. Сложность операций

| Операция | Временная | Пространственная |
|----------|-----------|------------------|
| Insert | O(m) | O(m) в худшем случае |
| Search | O(m) | O(1) |
| StartsWith | O(m) | O(1) |
| Delete | O(m) | O(1) |
| Автодополнение | O(m + k) | O(k) |

Где:
- m — длина слова/префикса
- k — количество результатов автодополнения

### 4.1 Сравнение с другими структурами

| Структура | Insert | Search | Prefix Search | Memory |
|-----------|--------|--------|---------------|--------|
| Trie | O(m) | O(m) | O(m) | O(ALPHABET × N × m) |
| Hash Table | O(m) avg | O(m) avg | O(n × m) | O(N × m) |
| Balanced BST | O(m log n) | O(m log n) | O(n × m) | O(N × m) |
| Sorted Array | O(n × m) | O(m log n) | O(m log n + k) | O(N × m) |

Где:
- N — количество слов
- n — количество слов в структуре
- m — длина слова

**Ключевое преимущество Trie**: поиск по префиксу за O(m), независимо от количества слов.

---

## 5. Расширения и вариации

### 5.1 Ассоциативный массив на Trie (Trie Map)

Вместо простого маркера `isEndOfWord` храним значение:

```cpp
template<typename V>
struct TrieMapNode {
    std::array<TrieMapNode*, 26> children{};
    std::optional<V> value;  // значение, если это конец ключа
};
```

**API:**
```cpp
template<typename V>
class TrieMap {
public:
    void put(const std::string& key, const V& value);
    std::optional<V> get(const std::string& key);
    bool remove(const std::string& key);
    std::vector<std::pair<std::string, V>> getByPrefix(const std::string& prefix);
};
```

### 5.2 Compressed Trie (Radix Tree)

Сжатие цепочек узлов с одним потомком:

```
Стандартный Trie:          Radix Tree:
     root                    root
      |                       |
      r                     "romane"
      |                      / \
      o                   "us"  "ulus"
      |
      m
      |
      a
      |
      n
     / \
    e   u
    |   |
    [end] l
          |
          u
          |
          s
          [end]
```

### 5.3 Ternary Search Tree (TST)

Компромисс между Trie и BST:
- Каждый узел имеет 3 указателя: left, middle, right
- Middle — переход к следующему символу
- Left/Right — BST по текущему символу

### 5.4 DAFSA (Directed Acyclic Word Graph)

Минимизированный Trie, где общие суффиксы также объединены.

---

## 6. Оптимизации памяти

### 6.1 Проблема памяти

Для алфавита размера A и N узлов требуется O(A × N) указателей.

Для 26 символов и 100,000 узлов:
- 26 × 100,000 × 8 байт ≈ 20 МБ только на указатели

### 6.2 Решения

**1. Хэш-таблица вместо массива:**
```cpp
std::unordered_map<char, std::unique_ptr<TrieNode>> children;
```

**2. Bitset для проверки наличия:**
```cpp
struct CompactNode {
    std::bitset<26> hasChild;
    std::vector<TrieNode*> children;  // только существующие
};
```

**3. Array of structs → Struct of arrays:**
```cpp
// Все узлы в одном массиве
std::vector<uint32_t> children;  // children[node_id * 26 + char_index]
std::vector<bool> isEnd;
```

---

## 7. Автодополнение

### 7.1 Алгоритм

```cpp
std::vector<std::string> autocomplete(const std::string& prefix, int limit) {
    TrieNode* node = findNode(prefix);
    if (!node) return {};
    
    std::vector<std::string> results;
    std::string current = prefix;
    collectWords(node, current, results, limit);
    return results;
}

void collectWords(TrieNode* node, std::string& current, 
                  std::vector<std::string>& results, int limit) {
    if (results.size() >= limit) return;
    
    if (node->isEndOfWord) {
        results.push_back(current);
    }
    
    for (int i = 0; i < 26; i++) {
        if (node->children[i]) {
            current.push_back('a' + i);
            collectWords(node->children[i], current, results, limit);
            current.pop_back();
        }
    }
}
```

### 7.2 Ранжирование результатов

Для более релевантных результатов можно добавить:
- Частоту использования слова
- Длину слова (короткие предпочтительнее)
- Контекст пользователя

```cpp
struct TrieNode {
    std::array<TrieNode*, 26> children{};
    bool isEndOfWord = false;
    uint32_t frequency = 0;  // для ранжирования
};
```

---

## 8. Практические задачи

### 8.1 LeetCode задачи

| # | Название | Сложность |
|---|----------|-----------|
| 208 | Implement Trie | Medium |
| 211 | Design Add and Search Words | Medium |
| 212 | Word Search II | Hard |
| 648 | Replace Words | Medium |
| 677 | Map Sum Pairs | Medium |
| 720 | Longest Word in Dictionary | Medium |
| 1268 | Search Suggestions System | Medium |

### 8.2 Типичные паттерны

**1. Поиск слова с wildcard (.):**
```cpp
bool searchWithWildcard(const std::string& pattern) {
    return dfs(root, pattern, 0);
}

bool dfs(TrieNode* node, const std::string& pattern, int i) {
    if (!node) return false;
    if (i == pattern.size()) return node->isEndOfWord;
    
    if (pattern[i] == '.') {
        for (auto child : node->children) {
            if (dfs(child, pattern, i + 1)) return true;
        }
        return false;
    }
    
    return dfs(node->children[pattern[i] - 'a'], pattern, i + 1);
}
```

**2. Замена слов на корни (Replace Words):**
```cpp
std::string replaceWords(const std::vector<std::string>& roots, 
                         const std::string& sentence) {
    // Построить Trie из корней
    // Для каждого слова в предложении найти кратчайший префикс-корень
}
```

---

## 9. Сравнение с Hash Table

### 9.1 Когда использовать Trie

✅ **Trie лучше:**
- Нужен поиск по префиксу
- Автодополнение
- Упорядоченный обход ключей
- Поиск всех ключей с общим префиксом
- Общие префиксы экономят память

❌ **Hash Table лучше:**
- Только точный поиск (без префиксов)
- Ключи не строки (или очень длинные)
- Важна простота реализации
- Нет паттерна общих префиксов

### 9.2 Бенчмарк-сценарии

| Сценарий | Trie | Hash Table |
|----------|------|------------|
| Вставка N слов | O(N × m) | O(N × m) avg |
| Точный поиск | O(m) | O(m) avg |
| Prefix search | O(m) | O(N × m) |
| Память (похожие слова) | Компактнее | Полные копии |
| Память (разные слова) | Больше | Компактнее |

---

## 10. Заключение

### 10.1 Ключевые выводы

1. **Trie** — специализированная структура для работы со строками
2. **O(m)** для всех базовых операций, независимо от количества слов
3. **Prefix search** — главное преимущество над hash table
4. **Память** — основной недостаток, решается оптимизациями
5. **Практическое применение**: автодополнение, spell-check, IP routing

### 10.2 Рекомендации по реализации

- Для учебных целей: массив фиксированного размера
- Для production: `unordered_map` или compressed trie
- Для максимальной производительности: array-based с bitset

---

## Литература

1. Knuth D. "The Art of Computer Programming, Vol. 3: Sorting and Searching"
2. Sedgewick R. "Algorithms in C++, Part 5: Graph Algorithms"
3. LeetCode Problem #208: Implement Trie
4. Wikipedia: Trie, Radix tree, Ternary search tree