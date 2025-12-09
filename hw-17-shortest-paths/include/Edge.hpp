#pragma once

/**
 * @brief Ребро графа (результат MST)
 * 
 * Содержит пару вершин, которые соединяет ребро.
 * Используется как выходной формат для алгоритмов MST.
 */
struct Edge {
    int v1;  ///< Первая вершина
    int v2;  ///< Вторая вершина
    
    Edge() : v1(-1), v2(-1) {}
    Edge(int vertex1, int vertex2) : v1(vertex1), v2(vertex2) {}
    
    bool operator==(const Edge& other) const {
        return (v1 == other.v1 && v2 == other.v2) ||
               (v1 == other.v2 && v2 == other.v1);
    }
};
