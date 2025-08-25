# Алгоритмы сортировки: Быстрая, Слиянием и Внешняя сортировка

***

## Метод «Разделяй и властвуй»

- Задача разбивается на подзадачи, которые решаются по отдельности.
- Итогом является объединение решений подзадач.
- Эффективность зависит от скорости разбиения, решения подзадач без дальнейшего деления и сборки решения.

***

## Быстрая сортировка (Quick Sort)

### Описание:

- Выбирается опорный элемент (чаще последний).
- Разделение массива на части: меньше опорного, равные, больше опорного.
- Рекурсивная сортировка частей без опорного.
- Известна высокой скоростью и эффективностью при среднем случае.


### Характеристики:

- Стабильность: Нет
- Адаптивность: Нет
- Онлайн: Нет
- Сложность: лучший и средний O(N log N), худший O(N²)


### Пример (C++17, ООП):

```cpp
#include <vector>
#include <iostream>

class QuickSort {
public:
    void sort(std::vector<int>& arr) {
        if (arr.empty()) return;
        quickSort(arr, 0, int(arr.size()) - 1);
    }
private:
    int partition(std::vector<int>& arr, int low, int high) {
        int pivot = arr[high];
        int i = low - 1;
        for (int j = low; j < high; ++j) {
            if (arr[j] <= pivot) {
                ++i;
                std::swap(arr[i], arr[j]);
            }
        }
        std::swap(arr[i + 1], arr[high]);
        return i + 1;
    }

    void quickSort(std::vector<int>& arr, int low, int high) {
        if (low < high) {
            int pi = partition(arr, low, high);
            quickSort(arr, low, pi - 1);
            quickSort(arr, pi + 1, high);
        }
    }
};
```


***

## Сортировка слиянием (Merge Sort)

### Описание:

- Рекурсивное деление массива пополам до единичных элементов.
- Сортировка половин и их слияние в отсортированный массив.
- Слияние происходит за линейное время.


### Характеристики:

- Стабильность: Да
- Адаптивность: Нет
- Онлайн: Нет
- Сложность: O(N log N) во всех случаях
- Требует дополнительную память O(N)


### Пример (C++17, ООП):

```cpp
#include <vector>
#include <iostream>

class MergeSort {
public:
    void sort(std::vector<int>& arr) {
        if (arr.empty()) return;
        mergeSort(arr, 0, int(arr.size()) - 1);
    }
private:
    void mergeSort(std::vector<int>& arr, int left, int right) {
        if (left >= right) return;
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }

    void merge(std::vector<int>& arr, int left, int mid, int right) {
        std::vector<int> temp(right - left + 1);
        int i = left, j = mid + 1, k = 0;
        while (i <= mid && j <= right) {
            if (arr[i] <= arr[j]) temp[k++] = arr[i++];
            else temp[k++] = arr[j++];
        }
        while (i <= mid) temp[k++] = arr[i++];
        while (j <= right) temp[k++] = arr[j++];
        for (int idx = 0; idx < temp.size(); ++idx) arr[left + idx] = temp[idx];
    }
};
```


***

## Внешняя сортировка (External Sorting)

### Общее описание:

- Применяется для сортировки очень больших наборов данных, которые не помещаются полностью в оперативную память.
- Использует внешние накопители (например, жесткие диски) для хранения данных.
- Часто реализуется с помощью **внешней сортировки слиянием (External Merge Sort)**.


### Как работает внешний метод сортировки слиянием:

1. **Разбиение (Run Generation):**
Данные считываются порциями, которые помещаются в память, сортируются внутренним алгоритмом (например, Merge Sort или Quick Sort), и записываются обратно на диск как отсортированные "прогоны" (runs).
2. **Слияние (Merge Phase):**
Отсортированные прогоны сливаются в несколько этапов, где на каждом этапе сливается ограниченное количество прогонов одновременно, чтобы суммарный объем данных в память не превышал её размер.
3. Этот процесс слияния продолжается, пока не будет получен полностью отсортированный файл.

### Особенности:

- Важным параметром является количество доступной оперативной памяти для одновременного чтения и записи данных.
- Эффективность зависит от количества проходов чтения/записи диска.
- Используются структуры данных, оптимизирующие операции чтения/записи (буферы).


### Примерный код внешней сортировки (класс и основные этапы) на C++ (с использованием STL и ООП):

```cpp
#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <string>
#include <algorithm>

class ExternalSort {
    size_t memoryLimit; // Максимальное количество элементов, помещающихся в память

public:
    explicit ExternalSort(size_t memLimit) : memoryLimit(memLimit) {}

    // Генерация частей (прогонов) с сортировкой во внутренней памяти
    std::vector<std::string> createRuns(const std::string& inputFile) {
        std::ifstream in(inputFile);
        std::vector<std::string> runFiles;
        std::vector<int> buffer;
        buffer.reserve(memoryLimit);

        int value;
        size_t runCount = 0;

        while (in >> value) {
            buffer.push_back(value);
            if (buffer.size() == memoryLimit) {
                std::sort(buffer.begin(), buffer.end());
                std::string runFile = "run_" + std::to_string(runCount++) + ".txt";
                std::ofstream out(runFile);
                for (int v : buffer) out << v << "\n";
                out.close();
                runFiles.push_back(runFile);
                buffer.clear();
            }
        }

        if (!buffer.empty()) {
            std::sort(buffer.begin(), buffer.end());
            std::string runFile = "run_" + std::to_string(runCount++) + ".txt";
            std::ofstream out(runFile);
            for (int v : buffer) out << v << "\n";
            out.close();
            runFiles.push_back(runFile);
        }
        return runFiles;
    }

    // Слияние нескольких прогонов (k-way merge)
    void mergeRuns(const std::vector<std::string>& runFiles, const std::string& outputFile) {
        auto compare = [](const std::pair<int, size_t>& a, const std::pair<int, size_t>& b) {
            return a.first > b.first;
        };

        std::vector<std::ifstream> inputs(runFiles.size());
        for (size_t i = 0; i < runFiles.size(); ++i) {
            inputs[i].open(runFiles[i]);
        }

        std::priority_queue<std::pair<int, size_t>, std::vector<std::pair<int, size_t>>, decltype(compare)> minHeap(compare);

        // Инициализация мин-кучи первым элементом каждого файла
        for (size_t i = 0; i < inputs.size(); ++i) {
            int val;
            if (inputs[i] >> val) {
                minHeap.emplace(val, i);
            }
        }

        std::ofstream out(outputFile);

        while (!minHeap.empty()) {
            auto [val, idx] = minHeap.top();
            minHeap.pop();
            out << val << "\n";
            int nextVal;
            if (inputs[idx] >> nextVal) {
                minHeap.emplace(nextVal, idx);
            }
        }

        for (auto& in : inputs) {
            in.close();
        }
        out.close();
    }
};
```


***

## Отличия внешней сортировки и внутренней

| Критерий | Внутренняя сортировка | Внешняя сортировка |
| :-- | :-- | :-- |
| Объем данных | Уместен в ОЗУ | Свыше объема оперативной памяти |
| Используемые носители | ОЗУ | Внешние накопители (диски) |
| Основные операции | Быстрый доступ к памяти | Медленный доступ, буферизация |
| Алгоритмы | QuickSort, MergeSort и др. | Внешний MergeSort и др. |
| Оптимизация | Использование кэша | Минимизация количества обращений к диску |


***

## Ссылки на источники

- https://en.wikipedia.org/wiki/External_sorting
- https://www.geeksforgeeks.org/external-sorting/
- https://www.programiz.com/dsa/external-sorting

***

# Общий вывод

- Методы быстрой (Quick Sort) и слиянием (Merge Sort) сортировки эффективны для внутренней сортировки в памяти.
- Внешняя сортировка используется для больших данных, не помещающихся в оперативную память, с ключевой методикой — внешним слиянием.
- Примеры реализации построены с учетом принципов ООП, SOLID и современных возможностей C++17, с использованием STL.

***


