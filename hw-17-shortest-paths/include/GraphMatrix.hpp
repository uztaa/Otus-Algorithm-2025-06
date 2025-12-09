#pragma once

#include "FactorArray.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <iomanip>

/**
 * @brief Матрица смежности для взвешенного графа
 * 
 * Хранит данные в одномерном FactorArray размером n*n.
 * Индекс элемента [i][j] вычисляется как i * n + j.
 * Поддерживает ориентированные и неориентированные графы.
 * 
 * @note Эта структура аналогична GraphMatrix из ДЗ-16.
 *       В будущем рекомендуется вынести в общую библиотеку graphlib.
 */
class GraphMatrix {
public:
    /// Значение, обозначающее отсутствие ребра
    static constexpr int INF = 1'000'000'000;

    /**
     * @brief Конструктор
     * @param vertices Количество вершин графа
     * @throws std::invalid_argument Если vertices < 0
     */
    explicit GraphMatrix(int vertices) : vertexCount_(vertices) {
        if (vertices < 0) {
            throw std::invalid_argument("vertices must be >= 0");
        }
        
        // Инициализация одномерного массива размером n*n значениями INF
        int totalSize = vertexCount_ * vertexCount_;
        for (int i = 0; i < totalSize; ++i) {
            data_.add(INF, i);
        }
    }

    /**
     * @brief Получить количество вершин
     */
    int size() const {
        return vertexCount_;
    }

    /**
     * @brief Получить вес ребра (u, v)
     * @return Вес ребра или INF если ребра нет
     * @throws std::out_of_range Если индексы вне диапазона
     */
    int getWeight(int u, int v) const {
        validateIndex(u);
        validateIndex(v);
        return data_.get(u * vertexCount_ + v);
    }

    /**
     * @brief Установить вес ребра (u, v)
     * @note Для неориентированного графа нужно вызвать дважды
     */
    void setWeight(int u, int v, int weight) {
        validateIndex(u);
        validateIndex(v);
        int idx = u * vertexCount_ + v;
        data_.remove(idx);
        data_.add(weight, idx);
    }

    /**
     * @brief Добавить ориентированное ребро
     */
    void addDirectedEdge(int u, int v, int weight) {
        setWeight(u, v, weight);
    }

    /**
     * @brief Добавить неориентированное ребро
     */
    void addUndirectedEdge(int u, int v, int weight) {
        setWeight(u, v, weight);
        setWeight(v, u, weight);
    }

    /**
     * @brief Удалить ребро (установить INF)
     */
    void removeEdge(int u, int v) {
        setWeight(u, v, INF);
    }

    /**
     * @brief Проверить наличие ребра
     */
    bool hasEdge(int u, int v) const {
        return getWeight(u, v) != INF;
    }

    /**
     * @brief Вывод матрицы для отладки
     */
    void print(std::ostream& os = std::cout, 
               const std::string& label = "GraphMatrix") const {
        os << "--- " << label << " (n=" << vertexCount_ << ") ---\n";
        
        // Заголовок
        os << "     ";
        for (int j = 0; j < vertexCount_; ++j) {
            os << std::setw(4) << j;
        }
        os << "\n";

        for (int i = 0; i < vertexCount_; ++i) {
            os << std::setw(3) << i << ": ";
            for (int j = 0; j < vertexCount_; ++j) {
                int w = getWeight(i, j);
                if (w == INF) {
                    os << std::setw(4) << "INF";
                } else {
                    os << std::setw(4) << w;
                }
            }
            os << "\n";
        }
        os << std::string(30, '-') << "\n";
    }

private:
    int vertexCount_;
    FactorArray<int> data_;  ///< Одномерный массив размером n*n

    void validateIndex(int idx) const {
        if (idx < 0 || idx >= vertexCount_) {
            throw std::out_of_range("vertex index out of range");
        }
    }
};