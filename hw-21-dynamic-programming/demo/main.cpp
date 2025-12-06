#include "Task1_Fractions.hpp"
#include "Task2_Garland.hpp"
#include "Task3_FiveEight.hpp"
#include "Task4_Islands.hpp"
#include "Task5_SmallBarn.hpp"
#include "Task6_BarnHeight.hpp"
#include "Task7_BarnWidth.hpp"
#include "Task8_LargeBarn.hpp"

#include <iostream>
#include <iomanip>

// Функция для вывода заголовка раздела
void printSection(const std::string& title) {
    std::cout << "\n";
    for (int i = 0; i < 60; ++i) {
        std::cout << "=";
    }
    std::cout << "\n";
    std::cout << "  " << title << "\n";
    for (int i = 0; i < 60; ++i) {
        std::cout << "=";
    }
    std::cout << "\n";
}

int main() {
    std::cout << "Homework 21: Dynamic Programming\n";
    std::cout << "================================\n\n";
    
    // JUNIOR part (4 tasks)
    
    // Task 1: Fractions
    // Решает задачу сложения двух дробей и сокращения результата
    // Использует алгоритм Евклида для нахождения НОД
    printSection("Task 1: Fractions (GCD and addition)");
    {
        std::string result1 = TaskOneFractions::execute("1/2+1/3");
        std::cout << "Input: \"1/2+1/3\"\n";
        std::cout << "Output: \"" << result1 << "\"\n";
        std::cout << "Expected: \"5/6\"\n";
        
        std::string result2 = TaskOneFractions::execute("1/4+1/4");
        std::cout << "\nInput: \"1/4+1/4\"\n";
        std::cout << "Output: \"" << result2 << "\"\n";
        std::cout << "Expected: \"1/2\"\n";
    }
    
    // Task 2: Garland
    // Находит максимальную сумму элементов при движении по треугольной матрице
    // DP на треугольнике: dp[i][j] = tree[i][j] + max(dp[i-1][j-1], dp[i-1][j])
    printSection("Task 2: Garland (Triangle DP)");
    {
        std::vector<std::vector<int>> tree = {
            {1},
            {2, 3},
            {4, 5, 6}
        };
        int result = TaskTwoGarland::execute(tree);
        std::cout << "Triangle:\n";
        std::cout << "      1\n";
        std::cout << "     2 3\n";
        std::cout << "    4 5 6\n";
        std::cout << "Maximum sum: " << result << "\n";
        std::cout << "Expected: 10 (path: 1 -> 3 -> 6)\n";
    }
    
    // Task 3: Five-Eight
    // Подсчитывает N-значные числа из цифр {5, 8} без трех одинаковых подряд
    // DP со состояниями: отслеживаем две последние цифры
    printSection("Task 3: Five-Eight (State DP)");
    {
        for (int n = 1; n <= 5; ++n) {
            long long result = TaskTheFiveEight::execute(n);
            std::cout << "N=" << n << ": " << result << " valid numbers\n";
        }
        std::cout << "\nFor N=3, expected 6 (558, 585, 588, 855, 858, 885)\n";
    }
    
    // Task 4: Islands
    // Считает количество компонент связности единиц в матрице (остров = компонента)
    // Использует DFS для обхода всей компоненты
    printSection("Task 4: Islands (DFS)");
    {
        std::vector<std::vector<int>> matrix = {
            {1, 0, 1},
            {0, 1, 1},
            {1, 0, 1}
        };
        int result = TaskFourIslands::execute(matrix);
        std::cout << "Matrix:\n";
        std::cout << "1 0 1\n";
        std::cout << "0 1 1\n";
        std::cout << "1 0 1\n";
        std::cout << "Number of islands: " << result << "\n";
        std::cout << "Expected: 3\n";
    }
    
    // MIDDLE part (4 stages)
    
    // Task 5: Small Barn
    // Находит максимальный прямоугольник из нулей в матрице
    // Брутфорс: перебираем все возможные углы и проверяем каждый прямоугольник
    printSection("Task 5: Small Barn (Brute force)");
    {
        std::vector<std::vector<int>> matrix = {
            {0, 0, 1},
            {0, 0, 0},
            {1, 0, 0}
        };
        int result = TaskFiveSmallBarn::execute(matrix);
        std::cout << "Matrix 3x3:\n";
        std::cout << "0 0 1\n";
        std::cout << "0 0 0\n";
        std::cout << "1 0 0\n";
        std::cout << "Maximum rectangle area: " << result << "\n";
    }
    
    // Task 6: Barn Height
    // Вычисляет для каждой клетки количество свободных клеток вверх
    // DP: height[i][j] = height[i-1][j] + 1 если свободна, иначе 0
    printSection("Task 6: Barn Height (DP heights)");
    {
        std::vector<std::pair<int, int>> occupied;
        // 3x3 matrix without obstacles
        auto heights = TaskSixBarnHeight::execute(3, 3, occupied);
        std::cout << "Height matrix (3x3, no obstacles):\n";
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                std::cout << heights[i][j] << " ";
            }
            std::cout << "\n";
        }
    }
    
    // Task 7: Barn Width
    // Находит левую и правую границы для каждого элемента массива высот
    // Монотонный стек: O(N) решение вместо O(N^2)
    printSection("Task 7: Barn Width (Monotonic stack)");
    {
        std::vector<int> height = {2, 1, 5, 6, 2, 3};
        std::vector<int> L, R;
        TaskSevenBarnWidth::execute(height, L, R);
        
        std::cout << "Height array: ";
        for (int h : height) {
            std::cout << h << " ";
        }
        std::cout << "\n";
        
        std::cout << "Left boundaries L: ";
        for (int l : L) {
            std::cout << l << " ";
        }
        std::cout << "\n";
        
        std::cout << "Right boundaries R: ";
        for (int r : R) {
            std::cout << r << " ";
        }
        std::cout << "\n";
    }
    
    // Task 8: Large Barn
    // Объединяет все три этапа: высоты + гистограмма + максимальный прямоугольник
    // Находит максимальный прямоугольник в матрице с препятствиями
    printSection("Task 8: Large Barn (Complete solution)");
    {
        std::vector<std::pair<int, int>> occupied = {
            {0, 2},  // Obstacle at (0, 2)
            {2, 0}   // Obstacle at (2, 0)
        };
        int result = TaskEightLargeBarn::execute(3, 3, occupied);
        std::cout << "Matrix 3x3 with obstacles at (0,2) and (2,0):\n";
        std::cout << "0 0 1\n";
        std::cout << "0 0 0\n";
        std::cout << "1 0 0\n";
        std::cout << "Maximum rectangle area: " << result << "\n";
    }
    
    // Summary section
    printSection("Summary");
    std::cout << "JUNIOR part: 4 tasks (4 bytes)\n";
    std::cout << "MIDDLE part: 4 stages (6 bytes)\n";
    std::cout << "TOTAL: 8 tasks (10 bytes)\n";
    
    return 0;
}
