#pragma once

#include <vector>

/**
 * @brief Ширина сарая - вычисление левых и правых границ
 * 
 * Решает задачу: для каждого элемента массива высот найти
 * - L[j]: индекс левого элемента с высотой >= A[j]
 * - R[j]: индекс правого элемента с высотой >= A[j]
 * 
 * Алгоритм:
 * Используется монотонный стек для O(n) решения
 * - Проходим слева направо для L[]
 * - Проходим справа налево для R[]
 * 
 * Временная сложность: O(N)
 * Пространственная сложность: O(N)
 */
class TaskSevenBarnWidth {
public:
    /**
     * @brief Вычислить левые и правые границы
     * 
     * @param height Массив высот
     * @param L Выходной массив левых границ
     * @param R Выходной массив правых границ
     */
    static void execute(const std::vector<int>& height,
                       std::vector<int>& L,
                       std::vector<int>& R);

private:
    /**
     * @brief Вычислить левые границы (монотонный стек)
     */
    static void computeLeft(const std::vector<int>& height,
                           std::vector<int>& L);
    
    /**
     * @brief Вычислить правые границы (монотонный стек)
     */
    static void computeRight(const std::vector<int>& height,
                            std::vector<int>& R);
};
