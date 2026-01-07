# Лекция: Алгоритмы сжатия данных

## 1. Введение

**Сжатие данных** — процесс преобразования информации с целью уменьшения её объёма. Используется для экономии места на диске и ускорения передачи данных.

### Типы сжатия

| Тип | Описание | Примеры |
|-----|----------|---------|
| **Без потерь** | Исходные данные восстанавливаются полностью | ZIP, GZIP, PNG |
| **С потерями** | Часть информации теряется | JPEG, MP3, H.264 |

### Ключевые метрики

- **Коэффициент сжатия** = размер_до / размер_после
- **Степень сжатия** = 1 - (размер_после / размер_до) × 100%
- **Скорость сжатия/распаковки** (МБ/с)

---

## 2. Run-Length Encoding (RLE)

### 2.1 Идея

Заменяем последовательности одинаковых символов парой (количество, символ).

```
Вход:  AAABBBCCCCCA
Выход: 3A3B5C1A
```

### 2.2 Простая реализация

```cpp
std::vector<uint8_t> compressRLE(const std::vector<uint8_t>& data) {
    std::vector<uint8_t> result;
    
    size_t i = 0;
    while (i < data.size()) {
        uint8_t current = data[i];
        uint8_t count = 1;
        
        // Считаем длину серии (максимум 255)
        while (i + count < data.size() && 
               data[i + count] == current && 
               count < 255) {
            count++;
        }
        
        result.push_back(count);
        result.push_back(current);
        i += count;
    }
    
    return result;
}
```

### 2.3 Проблема простого RLE

Для данных без повторов размер **удваивается**:

```
Вход:  ABCDEF (6 байт)
Выход: 1A1B1C1D1E1F (12 байт)
```

### 2.4 Улучшенный RLE — PackBits

Алгоритм PackBits (стандарт TIFF/MacPaint) решает проблему:

**Формат:**
- Флаговый байт N определяет режим:
  - N ∈ [0, 127]: следующие (N+1) байт — литералы (копируем как есть)
  - N ∈ [-127, -1]: следующий байт повторяется (1-N) раз
  - N = -128: пропускается (NOP)

```
Вход:  AAABCDEF
       ^^^      → серия 3×A: флаг=-2, байт=A
          ^^^^^ → литералы BCDEF: флаг=4, байты=BCDEF

Выход: FE 41 04 42 43 44 45 46
       -2  A  4  B  C  D  E  F
```

### 2.5 Реализация PackBits

```cpp
std::vector<uint8_t> compressPackBits(const std::vector<uint8_t>& data) {
    std::vector<uint8_t> result;
    std::vector<uint8_t> literals;
    
    auto flushLiterals = [&]() {
        while (!literals.empty()) {
            size_t count = std::min(literals.size(), size_t(128));
            result.push_back(static_cast<uint8_t>(count - 1));
            for (size_t j = 0; j < count; j++) {
                result.push_back(literals[j]);
            }
            literals.erase(literals.begin(), literals.begin() + count);
        }
    };
    
    size_t i = 0;
    while (i < data.size()) {
        // Ищем серию повторов
        size_t runLen = 1;
        while (i + runLen < data.size() && 
               data[i + runLen] == data[i] && 
               runLen < 128) {
            runLen++;
        }
        
        if (runLen >= 3) {
            // Выгодно кодировать как серию
            flushLiterals();
            result.push_back(static_cast<uint8_t>(1 - runLen));  // -2..-127
            result.push_back(data[i]);
            i += runLen;
        } else {
            // Добавляем к литералам
            literals.push_back(data[i]);
            i++;
        }
    }
    
    flushLiterals();
    return result;
}
```

### 2.6 Когда RLE эффективен

| Тип данных | Эффективность | Причина |
|------------|---------------|---------|
| Чёрно-белые изображения | ★★★★★ | Длинные серии 0 и 255 |
| Иконки, скриншоты | ★★★★☆ | Много однотонных областей |
| Текст | ★★☆☆☆ | Мало повторов |
| Фотографии | ★☆☆☆☆ | Случайный шум |
| Сжатые данные (ZIP) | ☆☆☆☆☆ | Уже максимально сжаты |

---

## 3. Алгоритм Хаффмана

### 3.1 Идея

Кодируем частые символы короткими кодами, редкие — длинными.

### 3.2 Построение дерева

1. Подсчитать частоту каждого символа
2. Создать лист для каждого символа с его частотой
3. Пока узлов больше одного:
   - Извлечь два узла с минимальной частотой
   - Создать родителя с суммой частот
   - Добавить родителя обратно

### 3.3 Пример

```
Текст: ABRACADABRA

Частоты: A=5, B=2, R=2, C=1, D=1

Дерево:
           (11)
          /    \
        (6)    A(5)
       /    \
     (3)    (3)
    /  \   /  \
   C(1) D(1) B(2) R(2)

Коды: A=1, B=010, R=011, C=000, D=001
```

### 3.4 Сжатие

```
Исходный текст: ABRACADABRA (11 символов × 8 бит = 88 бит)
Сжатый: 1-010-011-1-000-1-001-1-010-011-1 = 23 бита

Коэффициент сжатия: 88/23 ≈ 3.8
```

### 3.5 Код на C++

```cpp
struct HuffmanNode {
    uint8_t symbol;
    int frequency;
    HuffmanNode* left = nullptr;
    HuffmanNode* right = nullptr;
    
    bool isLeaf() const { return !left && !right; }
};

// Компаратор для priority_queue (min-heap)
struct CompareNodes {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        return a->frequency > b->frequency;
    }
};

HuffmanNode* buildTree(const std::map<uint8_t, int>& frequencies) {
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, CompareNodes> pq;
    
    // Создаём листья
    for (auto& [symbol, freq] : frequencies) {
        pq.push(new HuffmanNode{symbol, freq});
    }
    
    // Строим дерево
    while (pq.size() > 1) {
        HuffmanNode* left = pq.top(); pq.pop();
        HuffmanNode* right = pq.top(); pq.pop();
        
        HuffmanNode* parent = new HuffmanNode{0, left->frequency + right->frequency};
        parent->left = left;
        parent->right = right;
        
        pq.push(parent);
    }
    
    return pq.top();
}
```

---

## 4. Алгоритм LZ77

### 4.1 Идея

Ищем повторяющиеся последовательности в уже обработанных данных и ссылаемся на них.

### 4.2 Скользящее окно

- **Словарь** (search buffer): уже закодированные данные
- **Буфер просмотра** (lookahead buffer): данные для кодирования

```
| --- словарь --- | --- буфер просмотра --- |
|    ABRACADAB    |         RABRA           |
                  ↑
            текущая позиция
```

### 4.3 Формат токена

Каждый токен: `(offset, length, next_char)`

- **offset** — смещение назад к началу совпадения
- **length** — длина совпадения
- **next_char** — следующий символ после совпадения

### 4.4 Пример

```
Текст: ABRACADABRABRA

Кодирование:
Позиция 0: A → (0,0,A) — нет совпадений
Позиция 1: B → (0,0,B)
Позиция 2: R → (0,0,R)
Позиция 3: A → (3,1,C) — 'A' найдено на offset=3, следующий 'C'
Позиция 5: A → (2,1,D) — 'A' найдено на offset=2
Позиция 7: ABRA → (7,4,B) — 'ABRA' найдено на offset=7
...

Результат: (0,0,A)(0,0,B)(0,0,R)(3,1,C)(2,1,D)(7,4,B)(3,1,A)
```

### 4.5 Декодирование

```cpp
std::vector<uint8_t> decodeLZ77(const std::vector<Token>& tokens) {
    std::vector<uint8_t> result;
    
    for (const auto& token : tokens) {
        if (token.length > 0) {
            // Копируем из уже декодированных данных
            size_t start = result.size() - token.offset;
            for (size_t i = 0; i < token.length; i++) {
                result.push_back(result[start + i]);
            }
        }
        result.push_back(token.nextChar);
    }
    
    return result;
}
```

### 4.6 Применение

LZ77 — основа многих современных алгоритмов:
- **DEFLATE** (ZIP, GZIP, PNG) = LZ77 + Huffman
- **LZMA** (7-Zip, XZ)
- **LZ4** (быстрое сжатие)
- **Zstandard** (Facebook)

---

## 5. Сравнение алгоритмов

| Алгоритм | Сложность | Сжатие | Скорость | Применение |
|----------|-----------|--------|----------|------------|
| **RLE** | O(n) | Низкое | Очень быстро | Простые изображения |
| **Huffman** | O(n log n) | Среднее | Быстро | Текст, как часть других |
| **LZ77** | O(n × w) | Хорошее | Средне | Универсальное |
| **DEFLATE** | O(n × w) | Отличное | Средне | ZIP, PNG, HTTP |

где n — размер данных, w — размер окна

---

## 6. Энтропия и пределы сжатия

### 6.1 Энтропия Шеннона

**Энтропия** — минимальное среднее количество бит на символ:

```
H = -Σ p(x) × log₂(p(x))
```

Для равновероятных символов алфавита размера N:
```
H = log₂(N)
```

### 6.2 Пределы

- Случайные данные: H ≈ 8 бит/байт → **не сжимаются**
- Текст на английском: H ≈ 1.5-2 бит/символ → сжатие ~4x
- Уже сжатые данные: H ≈ 8 бит/байт → **не сжимаются повторно**

---

## 7. Практические рекомендации

1. **Для простых задач** — RLE (быстро, просто)
2. **Для текста** — DEFLATE (zlib)
3. **Для архивации** — LZMA (7-Zip) или Zstandard
4. **Для изображений** — PNG (без потерь) или JPEG (с потерями)
5. **Для потоковой передачи** — LZ4 (скорость важнее сжатия)

---

## 8. Литература

1. Сэломон Д. "Сжатие данных, изображений и звука"
2. Witten I., Moffat A., Bell T. "Managing Gigabytes"
3. RFC 1951 — DEFLATE Compressed Data Format
4. Колмогоров А.Н. — работы по теории информации

---

## 9. Итоги

- **RLE** — простейший алгоритм, эффективен для данных с повторами
- **PackBits** — улучшенный RLE, не раздувает данные без повторов
- **Huffman** — оптимальный префиксный код по частотам
- **LZ77** — словарное сжатие, основа современных архиваторов
- **Энтропия** — теоретический предел сжатия
