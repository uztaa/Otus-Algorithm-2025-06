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

/**
 * @brief Взвешенное ребро графа
 * 
 * Используется внутри алгоритмов для сортировки и обработки рёбер.
 */
struct WeightedEdge {
    int from;    ///< Начальная вершина
    int to;      ///< Конечная вершина
    int weight;  ///< Вес ребра
    
    WeightedEdge() : from(-1), to(-1), weight(0) {}
    WeightedEdge(int f, int t, int w) : from(f), to(t), weight(w) {}
    
    /**
     * @brief Преобразование в Edge (без веса)
     */
    Edge toEdge() const {
        return Edge(from, to);
    }
    
    /**
     * @brief Сравнение по весу (для сортировки)
     */
    bool operator<(const WeightedEdge& other) const {
        return weight < other.weight;
    }
    
    bool operator>(const WeightedEdge& other) const {
        return weight > other.weight;
    }
    
    bool operator==(const WeightedEdge& other) const {
        return from == other.from && to == other.to && weight == other.weight;
    }
};