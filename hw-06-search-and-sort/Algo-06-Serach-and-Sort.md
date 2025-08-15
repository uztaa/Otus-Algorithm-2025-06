# 📘 Обобщённый отчёт по алгоритмам сортировки и поиска с применением SOLID принципов

---

## ✨ Характеристики сортировок

* **Стабильность:** сохраняет ли порядок одинаковых элементов
* **Адаптивность:** использует ли предварительную упорядоченность массива
* **Онлайн:** может ли обрабатывать данные по поступлению

---

## 🔹 Сводная таблица сортировок

| Алгоритм              | Сложность  | Стабильность | Адаптивность | Онлайн | Применение                            |
| --------------------- | ---------- | ------------ | ------------ | ------ | ------------------------------------- |
| BubbleSorter          | O(n²)      | ✅            | ✅            | ❌      | Учебная, для наглядной демонстрации   |
| InsertionSorter       | O(n²)      | ✅            | ✅            | ✅      | Эффективен при почти отсортированных  |
| InsertionShiftSorter  | O(n²)      | ✅            | ✅            | ✅      | Более быстрая версия вставки          |
| BinaryInsertionSorter | O(n log n) | ✅            | ✅            | ✅      | Снижает число сравнений               |
| ShellSorter           | O(n log²n) | ❌            | ✅            | ❌      | Хорош в практике для средних массивов |

---

## 🧱 Реализация интерфейсов с SOLID

### 🔍 Интерфейс Searchable — для алгоритмов поиска

```cpp
/**
 * Интерфейс для алгоритмов поиска
 * @return Record* — указатель на найденную запись или nullptr
 */
class Searchable {
public:
    virtual Record* search(std::vector<Record>& arr, int key) = 0;
    virtual ~Searchable() = default;
};
```

### 🔄 Интерфейс Sortable — для алгоритмов сортировки

```cpp
/**
 * Интерфейс для алгоритмов сортировки
 * @return Пара (кол-во сравнений, кол-во обменов или сдвигов)
 */
class Sortable {
public:
    virtual std::pair<size_t, size_t> sort(std::vector<Record>& arr) = 0;
    virtual ~Sortable() = default;
};
```

### 🧾 Record — модель записи с инкапсуляцией

```cpp
class Record {
private:
    int key;
    std::string value;

public:
    Record(int k, std::string v) : key(k), value(std::move(v)) {}

    int getKey() const { return key; }
    const std::string& getValue() const { return value; }
};
```

---

## 🔎 Реализация алгоритмов поиска

### 🔹 Последовательный поиск

```cpp
class SequentialSearcher : public Searchable {
public:
    Record* search(std::vector<Record>& arr, int searchKey) override {
        for (auto& rec : arr) {
            if (rec.getKey() == searchKey)
                return &rec;
        }
        return nullptr;
    }
};
```

### 🔹 Метод транспозиции

```cpp
class TranspositionSearcher : public Searchable {
public:
    Record* search(std::vector<Record>& arr, int key) override {
        for (size_t i = 0; i < arr.size(); ++i) {
            if (arr[i].getKey() == key) {
                if (i > 0)
                    std::swap(arr[i], arr[i - 1]);
                return &arr[i > 0 ? i - 1 : i];
            }
        }
        return nullptr;
    }
};
```

### 🔹 Метод перемещения в начало

```cpp
class MoveToFrontSearcher : public Searchable {
public:
    Record* search(std::vector<Record>& arr, int key) override {
        for (size_t i = 0; i < arr.size(); ++i) {
            if (arr[i].getKey() == key) {
                Record found = arr[i];
                for (size_t j = i; j > 0; --j)
                    arr[j] = arr[j - 1];
                arr[0] = found;
                return &arr[0];
            }
        }
        return nullptr;
    }
};
```

---

## 🔄 Реализация алгоритмов сортировки

### 🔹 Пузырьковая сортировка

```cpp
class BubbleSorter : public Sortable {
public:
    std::pair<size_t, size_t> sort(std::vector<Record>& arr) override {
        size_t comparisons = 0, swaps = 0;
        for (size_t i = 0; i < arr.size(); ++i) {
            for (size_t j = arr.size() - 1; j > i; --j) {
                ++comparisons;
                if (arr[j].getKey() < arr[j - 1].getKey()) {
                    std::swap(arr[j], arr[j - 1]);
                    ++swaps;
                }
            }
        }
        return {comparisons, swaps};
    }
};
```

### 🔹 Сортировка вставками (с обменом)

```cpp
class InsertionSorter : public Sortable {
public:
    std::pair<size_t, size_t> sort(std::vector<Record>& arr) override {
        size_t comparisons = 0, swaps = 0;
        for (size_t i = 1; i < arr.size(); ++i) {
            size_t j = i;
            while (j > 0 && ++comparisons && arr[j].getKey() < arr[j - 1].getKey()) {
                std::swap(arr[j], arr[j - 1]);
                ++swaps;
                --j;
            }
        }
        return {comparisons, swaps};
    }
};
```

### 🔹 Сортировка вставками (со сдвигом)

```cpp
class InsertionShiftSorter : public Sortable {
public:
    std::pair<size_t, size_t> sort(std::vector<Record>& arr) override {
        size_t comparisons = 0, shifts = 0;
        for (size_t i = 1; i < arr.size(); ++i) {
            Record temp = arr[i];
            size_t j = i;
            while (j > 0 && ++comparisons && arr[j - 1].getKey() > temp.getKey()) {
                arr[j] = arr[j - 1];
                ++shifts;
                --j;
            }
            arr[j] = temp;
        }
        return {comparisons, shifts};
    }
};
```

### 🔹 Бинарная сортировка вставками

```cpp
class BinaryInsertionSorter : public Sortable {
private:
    size_t binarySearch(const std::vector<Record>& arr, const Record& key, size_t left, size_t right, size_t& comparisons) {
        while (left <= right) {
            size_t mid = (left + right) / 2;
            ++comparisons;
            if (key.getKey() < arr[mid].getKey())
                right = mid - 1;
            else
                left = mid + 1;
        }
        return left;
    }

public:
    std::pair<size_t, size_t> sort(std::vector<Record>& arr) override {
        size_t comparisons = 0, shifts = 0;
        for (size_t i = 1; i < arr.size(); ++i) {
            Record key = arr[i];
            size_t pos = binarySearch(arr, key, 0, i - 1, comparisons);
            for (size_t j = i; j > pos; --j) {
                arr[j] = arr[j - 1];
                ++shifts;
            }
            arr[pos] = key;
        }
        return {comparisons, shifts};
    }
};
```

### 🔹 Сортировка Шелла

```cpp
class ShellSorter : public Sortable {
public:
    std::pair<size_t, size_t> sort(std::vector<Record>& arr) override {
        size_t comparisons = 0, swaps = 0;
        for (size_t gap = arr.size() / 2; gap > 0; gap /= 2) {
            for (size_t i = gap; i < arr.size(); ++i) {
                size_t j = i;
                while (j >= gap && ++comparisons && arr[j - gap].getKey() > arr[j].getKey()) {
                    std::swap(arr[j], arr[j - gap]);
                    ++swaps;
                    j -= gap;
                }
            }
        }
        return {comparisons, swaps};
    }
};
```

---

## 🔎 Расширенные описания алгоритмов

* **BubbleSorter:** Учебный алгоритм, используемый в демонстрационных целях. Показывает суть обменов.
* **InsertionSorter:** Эффективен при небольших массивах и почти отсортированных данных.
* **BinaryInsertionSorter:** Уменьшает количество сравнений — применим в системах, где компаратор дорогой.
* **ShellSorter:** Подходит для embedded-устройств, компактный и надёжный без дополнительной памяти.
* **Поисковые методы:** Используются в LRU-кэшах, адаптивных таблицах, небольших БД без индексов.
