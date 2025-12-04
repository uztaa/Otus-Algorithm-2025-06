#pragma once

#include "WeightedEdge.hpp"
#include "FactorArray.h"

/**
 * @brief Результат работы алгоритма построения MST
 */
struct MSTResult {
    FactorArray<Edge> edges;  ///< Рёбра минимального остовного дерева
    int totalWeight;          ///< Суммарный вес MST
    bool isConnected;         ///< true если граф связный (MST построено полностью)
    
    MSTResult() : edges(), totalWeight(0), isConnected(true) {}
};