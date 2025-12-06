# Примеры кода и паттерны реализации

## Паттерн 1: Базовая структура класса

```cpp
// include/TaskX_Name.hpp
#pragma once

#include <vector>
#include <string>

/**
 * @brief Краткое описание задачи
 * 
 * Подробное описание того, что делает эта задача.
 * Объяснение алгоритма, сложность, примеры.
 * 
 * Алгоритм:
 * - Шаг 1: ...
 * - Шаг 2: ...
 * 
 * Временная сложность: O(...)
 * Пространственная сложность: O(...)
 * 
 * @example
 * TaskXName::execute(input) → output
 * 
 * @note Предусловия: 1 ≤ N ≤ 10000
 */
class TaskXName {
public:
    /**
     * @brief Основной метод решения
     * 
     * @param input Входные данные
     * @return ResultType Результат вычислений
     * 
     * @throws std::invalid_argument если входные данные некорректны
     */
    static ResultType execute(InputType input);

private:
    /**
     * @brief Вспомогательная функция
     * 
     * Описание того, что делает функция.
     */
    static HelperType helperFunction(HelperInputType param);
};
```

---

## Паттерн 2: Простая ДП (Fibonacci-style)

```cpp
// Task3_FiveEight.hpp (квадрат-free последовательности)
#pragma once

#include <vector>

class TaskTheFiveEight {
public:
    /**
     * @brief Найти количество N-значных чисел из {5, 8} без трёх подряд
     * 
     * ДП с состояниями:
     * dp[i][d1][d2] = кол-во i-значных чисел, где последние две цифры d1, d2
     * 
     * Переход:
     * - Если d1 ≠ d2 или d2 == 5/8 и d1 ≠ d2, добавляем либо 5, либо 8
     * - Запрещаем три подряд
     * 
     * @param n Количество цифр (1-88)
     * @return long long Количество допустимых чисел
     */
    static long long execute(int n);

private:
    // Состояния: 0 - начало, 5 - цифра 5, 8 - цифра 8
    static const int START = 0;
    static const int DIGIT_5 = 5;
    static const int DIGIT_8 = 8;
};
```

```cpp
// Task3_FiveEight.cpp
#include "Task3_FiveEight.hpp"

long long TaskTheFiveEight::execute(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 2;  // "5" и "8"
    
    // dp[i][d1][d2] - количество i-значных чисел
    // где d1 = предпредпоследняя, d2 = последняя
    // 0 = неопределено, 5 = 5, 8 = 8
    
    // Используем массивы или map для эффективности
    std::vector<std::vector<std::vector<long long>>> dp(
        n + 1,
        std::vector<std::vector<long long>>(3,
            std::vector<long long>(3, 0)
        )
    );
    
    // Базовый случай: 1 цифра
    dp[1][0][1] = 1;  // "5" (индекс 0, позиция 1)
    dp[1][0][2] = 1;  // "8" (индекс 0, позиция 2)
    
    // ДП переход
    for (int i = 2; i <= n; ++i) {
        for (int d1 = 0; d1 <= 2; ++d1) {
            for (int d2 = 1; d2 <= 2; ++d2) {
                if (dp[i-1][d1][d2] == 0) continue;
                
                // Попробовать добавить 5 (индекс 1)
                if (!(d1 == 1 && d2 == 1)) {  // не три пятёрки подряд
                    dp[i][d2][1] += dp[i-1][d1][d2];
                }
                
                // Попробовать добавить 8 (индекс 2)
                if (!(d1 == 2 && d2 == 2)) {  // не три восьмёрки подряд
                    dp[i][d2][2] += dp[i-1][d1][d2];
                }
            }
        }
    }
    
    // Сумма всех завершённых последовательностей
    long long result = 0;
    for (int d1 = 0; d1 <= 2; ++d1) {
        for (int d2 = 1; d2 <= 2; ++d2) {
            result += dp[n][d1][d2];
        }
    }
    
    return result;
}
```

---

## Паттерн 3: ДП на матрице (Garland)

```cpp
// Task2_Garland.hpp
#pragma once

#include <vector>

class TaskTwoGarland {
public:
    /**
     * @brief Найти максимальную сумму гирлянды в ёлочке
     * 
     * ДП на треугольнике:
     * dp[i][j] = максимальная сумма до позиции (i, j)
     * 
     * Переход:
     * dp[i][j] = tree[i][j] + max(dp[i-1][j-1], dp[i-1][j])
     * 
     * @param tree N×N треугольная матрица (tree[i].size() == i+1)
     * @return int Максимальная сумма
     */
    static int execute(const std::vector<std::vector<int>>& tree);
};
```

```cpp
// Task2_Garland.cpp
#include "Task2_Garland.hpp"
#include <algorithm>

int TaskTwoGarland::execute(const std::vector<std::vector<int>>& tree) {
    if (tree.empty()) return 0;
    
    int n = tree.size();
    
    // Оптимизация памяти: используем два массива вместо матрицы
    std::vector<int> prev(1, tree[0][0]);  // Предыдущая строка
    
    // Заполняем ДП снизу по строкам
    for (int i = 1; i < n; ++i) {
        std::vector<int> curr(i + 1);
        
        for (int j = 0; j <= i; ++j) {
            int max_prev = -1;
            
            // Проверяем диагональ слева (i-1, j-1)
            if (j > 0) {
                max_prev = std::max(max_prev, prev[j - 1]);
            }
            
            // Проверяем вертикаль (i-1, j)
            if (j < prev.size()) {
                max_prev = std::max(max_prev, prev[j]);
            }
            
            curr[j] = tree[i][j] + max_prev;
        }
        
        prev = curr;
    }
    
    // Максимум в последней строке
    return *std::max_element(prev.begin(), prev.end());
}
```

---

## Паттерн 4: Граф (DFS/BFS)

```cpp
// Task4_Islands.hpp
#pragma once

#include <vector>

class TaskFourIslands {
public:
    /**
     * @brief Найти количество островов в матрице
     * 
     * Алгоритм:
     * 1. Для каждой непосещённой единицы запускаем DFS
     * 2. DFS помечает всю компоненту как посещённую
     * 3. Увеличиваем счётчик островов
     * 
     * @param matrix N×N матрица (0 или 1)
     * @return int Количество островов
     */
    static int execute(std::vector<std::vector<int>>& matrix);

private:
    /**
     * @brief Глубинный поиск для пометки компоненты
     */
    static void dfs(int i, int j, std::vector<std::vector<int>>& matrix,
                    std::vector<std::vector<bool>>& visited, int n);
};
```

```cpp
// Task4_Islands.cpp
#include "Task4_Islands.hpp"

int TaskFourIslands::execute(std::vector<std::vector<int>>& matrix) {
    if (matrix.empty()) return 0;
    
    int n = matrix.size();
    std::vector<std::vector<bool>> visited(n, std::vector<bool>(n, false));
    int island_count = 0;
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (matrix[i][j] == 1 && !visited[i][j]) {
                dfs(i, j, matrix, visited, n);
                island_count++;
            }
        }
    }
    
    return island_count;
}

void TaskFourIslands::dfs(int i, int j, std::vector<std::vector<int>>& matrix,
                          std::vector<std::vector<bool>>& visited, int n) {
    if (i < 0 || i >= n || j < 0 || j >= n) return;
    if (visited[i][j] || matrix[i][j] == 0) return;
    
    visited[i][j] = true;
    
    // Четыре направления: вверх, вниз, влево, вправо
    dfs(i + 1, j, matrix, visited, n);  // вниз
    dfs(i - 1, j, matrix, visited, n);  // вверх
    dfs(i, j + 1, matrix, visited, n);  // вправо
    dfs(i, j - 1, matrix, visited, n);  // влево
}
```

---

## Паттерн 5: Монотонный стек (Barn Width)

```cpp
// Task7_BarnWidth.hpp
#pragma once

#include <vector>

class TaskSevenBarnWidth {
public:
    /**
     * @brief Вычислить левые и правые границы для массива высот
     * 
     * Для каждого элемента A[j] найти:
     * - L[j]: индекс левого элемента с высотой ≥ A[j]
     * - R[j]: индекс правого элемента с высотой ≥ A[j]
     * 
     * Используется монотонный стек для O(n).
     * 
     * @param height Массив высот
     * @param L Выходной массив левых границ
     * @param R Выходной массив правых границ
     */
    static void execute(const std::vector<int>& height,
                        std::vector<int>& L,
                        std::vector<int>& R);

private:
    static void computeLeft(const std::vector<int>& height,
                            std::vector<int>& L);
    
    static void computeRight(const std::vector<int>& height,
                             std::vector<int>& R);
};
```

```cpp
// Task7_BarnWidth.cpp
#include "Task7_BarnWidth.hpp"
#include <stack>

void TaskSevenBarnWidth::execute(const std::vector<int>& height,
                                  std::vector<int>& L,
                                  std::vector<int>& R) {
    computeLeft(height, L);
    computeRight(height, R);
}

void TaskSevenBarnWidth::computeLeft(const std::vector<int>& height,
                                      std::vector<int>& L) {
    int n = height.size();
    L.assign(n, 0);
    
    std::stack<int> st;  // Монотонно возрастающий стек индексов
    
    for (int j = 0; j < n; ++j) {
        // Удаляем элементы меньше текущего
        while (!st.empty() && height[st.top()] < height[j]) {
            st.pop();
        }
        
        if (st.empty()) {
            L[j] = 0;  // Нет меньших слева, от начала
        } else {
            L[j] = st.top() + 1;  // От следующего после найденного
        }
        
        st.push(j);
    }
}

void TaskSevenBarnWidth::computeRight(const std::vector<int>& height,
                                       std::vector<int>& R) {
    int n = height.size();
    R.assign(n, n - 1);
    
    std::stack<int> st;  // Монотонно возрастающий стек индексов
    
    for (int j = n - 1; j >= 0; --j) {
        // Удаляем элементы меньше текущего
        while (!st.empty() && height[st.top()] < height[j]) {
            st.pop();
        }
        
        if (st.empty()) {
            R[j] = n - 1;  // Нет меньших справа, до конца
        } else {
            R[j] = st.top() - 1;  // До предыдущего перед найденным
        }
        
        st.push(j);
    }
}
```

---

## Паттерн 6: Комбинирование этапов (Large Barn)

```cpp
// Task8_LargeBarn.hpp
#pragma once

#include <vector>
#include <utility>

class TaskEightLargeBarn {
public:
    /**
     * @brief Найти максимальную площадь прямоугольника в матрице с препятствиями
     * 
     * Объединяет все этапы:
     * 1. Вычисление высот (Этап 2)
     * 2. Нахождение максимума в гистограмме (Этап 4)
     * 
     * @param n Количество строк
     * @param m Количество столбцов
     * @param occupied Координаты занятых клеток
     * @return int Максимальная площадь
     */
    static int execute(int n, int m,
                       const std::vector<std::pair<int, int>>& occupied);

private:
    // Вычисление высот матрицы
    static std::vector<std::vector<int>> getHeights(
        int n, int m,
        const std::vector<std::pair<int, int>>& occupied);
    
    // Largest Rectangle in Histogram для одной строки
    static int maxRectangleInHistogram(const std::vector<int>& height);
};
```

```cpp
// Task8_LargeBarn.cpp
#include "Task8_LargeBarn.hpp"
#include <algorithm>
#include <stack>

int TaskEightLargeBarn::execute(
    int n, int m,
    const std::vector<std::pair<int, int>>& occupied) {
    
    // Этап 2: Вычисляем высоты
    auto heights = getHeights(n, m, occupied);
    
    int max_area = 0;
    
    // Для каждой строки (как основания гистограммы)
    for (int i = 0; i < n; ++i) {
        int area = maxRectangleInHistogram(heights[i]);
        max_area = std::max(max_area, area);
    }
    
    return max_area;
}

std::vector<std::vector<int>> TaskEightLargeBarn::getHeights(
    int n, int m,
    const std::vector<std::pair<int, int>>& occupied) {
    
    // Инициализация матрицы (по умолчанию свободно)
    std::vector<std::vector<int>> matrix(n, std::vector<int>(m, 0));
    
    // Пометить занятые клетки
    for (const auto& [x, y] : occupied) {
        if (x < n && y < m) {
            matrix[x][y] = 1;
        }
    }
    
    // ДП: вычисляем высоты
    std::vector<std::vector<int>> heights(n, std::vector<int>(m));
    
    for (int j = 0; j < m; ++j) {
        heights[0][j] = (matrix[0][j] == 0) ? 1 : 0;
    }
    
    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (matrix[i][j] == 1) {
                heights[i][j] = 0;
            } else {
                heights[i][j] = heights[i-1][j] + 1;
            }
        }
    }
    
    return heights;
}

int TaskEightLargeBarn::maxRectangleInHistogram(const std::vector<int>& height) {
    int n = height.size();
    if (n == 0) return 0;
    
    // Используем монотонный стек для O(n)
    std::vector<int> L(n), R(n);
    std::stack<int> st;
    
    // Вычисляем левые границы
    for (int j = 0; j < n; ++j) {
        while (!st.empty() && height[st.top()] < height[j]) {
            st.pop();
        }
        L[j] = st.empty() ? 0 : st.top() + 1;
        st.push(j);
    }
    
    // Очищаем стек
    while (!st.empty()) st.pop();
    
    // Вычисляем правые границы
    for (int j = n - 1; j >= 0; --j) {
        while (!st.empty() && height[st.top()] < height[j]) {
            st.pop();
        }
        R[j] = st.empty() ? n - 1 : st.top() - 1;
        st.push(j);
    }
    
    // Вычисляем максимальную площадь
    int max_area = 0;
    for (int j = 0; j < n; ++j) {
        int width = R[j] - L[j] + 1;
        int area = height[j] * width;
        max_area = std::max(max_area, area);
    }
    
    return max_area;
}
```

---

## Паттерн 7: Google Test структура

```cpp
// tests/Task2Test.cpp
#include <gtest/gtest.h>
#include "Task2_Garland.hpp"

class GarlandTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Инициализация перед каждым тестом
    }
};

TEST_F(GarlandTest, SingleElement) {
    std::vector<std::vector<int>> tree = {{5}};
    EXPECT_EQ(TaskTwoGarland::execute(tree), 5);
}

TEST_F(GarlandTest, Triangle) {
    std::vector<std::vector<int>> tree = {
        {1},
        {2, 3},
        {4, 5, 6}
    };
    // Максимум: 1 → 3 → 6 = 10
    EXPECT_EQ(TaskTwoGarland::execute(tree), 10);
}

TEST_F(GarlandTest, AllNegative) {
    std::vector<std::vector<int>> tree = {
        {-1},
        {-2, -3},
        {-4, -5, -6}
    };
    // Максимум: -1 + -2 + -4 = -7
    EXPECT_EQ(TaskTwoGarland::execute(tree), -7);
}

TEST_F(GarlandTest, Large) {
    // Генерируем большой треугольник
    int n = 100;
    std::vector<std::vector<int>> tree(n);
    for (int i = 0; i < n; ++i) {
        tree[i].resize(i + 1);
        for (int j = 0; j <= i; ++j) {
            tree[i][j] = i * 100 + j;  // Уникальные значения
        }
    }
    
    int result = TaskTwoGarland::execute(tree);
    EXPECT_GT(result, 0);
}
```

---

## Структура demo/main.cpp

```cpp
#include "Task1_Fractions.hpp"
#include "Task2_Garland.hpp"
// ... остальные include

#include <iostream>
#include <iomanip>

void printSection(const std::string& title) {
    std::cout << "\n" << std::string(50, '=') << "\n";
    std::cout << "  " << title << "\n";
    std::cout << std::string(50, '=') << "\n";
}

int main() {
    std::cout << "ДЗ-21: Динамическое программирование\n";
    std::cout << "======================================\n";
    
    // Task 1: Fractions
    printSection("Задача 1: Раз горох, два горох (Дроби)");
    std::string result1 = TaskOneFractions::execute("1/2+1/3");
    std::cout << "Вход: \"1/2+1/3\"\n";
    std::cout << "Выход: \"" << result1 << "\"\n";
    std::cout << "Ожидается: \"5/6\"\n";
    
    // Task 2: Garland
    printSection("Задача 2: Ёлочка программиста");
    std::vector<std::vector<int>> tree = {
        {1},
        {2, 3},
        {4, 5, 6}
    };
    int result2 = TaskTwoGarland::execute(tree);
    std::cout << "Максимальная сумма: " << result2 << "\n";
    
    // ... остальные задачи
    
    return 0;
}
```

---

**Эти паттерны — основа для реализации всех 8 задач. Адаптируйте их под конкретные требования.**