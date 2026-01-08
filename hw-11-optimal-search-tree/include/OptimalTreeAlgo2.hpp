#pragma once

#include "OptimalTreeBase.hpp"
#include <algorithm>
#include <vector>
#include <cmath>

/**
 * @brief Алгоритм 2 построения приближённого ДОП (метод "центра тяжести")
 * 
 * Идея: Выбирать корнем узел, который минимизирует разность весов
 * левого и правого поддеревьев. Рекурсивно применять для поддеревьев.
 * 
 * Алгоритм:
 * 1. Отсортировать ключи по возрастанию
 * 2. Найти "центр тяжести" — ключ Vk, где:
 *    - Σw[0..k-1] ≈ Σw[k+1..n-1]
 * 3. Vk становится корнем
 * 4. Рекурсивно построить левое поддерево из [0..k-1]
 * 5. Рекурсивно построить правое поддерево из [k+1..n-1]
 * 
 * Сложность: O(n log n) сортировка + O(n log n) построение = O(n log n)
 * 
 * Этот алгоритм асимптотически приближается к оптимальному при n→∞.
 */
class OptimalTreeAlgo2 : public OptimalTreeBase {
public:
    OptimalTreeAlgo2() = default;
    ~OptimalTreeAlgo2() override = default;

    std::string getName() const override { return "OptimalTree-A2"; }

    /**
     * @brief Построить дерево методом "центра тяжести"
     */
    void build(const std::vector<int>& keys, 
               const std::vector<int>& weights) override {
        if (keys.size() != weights.size()) {
            throw std::invalid_argument("Keys and weights must have same size");
        }
        if (keys.empty()) return;

        clear();

        // Создаём пары (ключ, вес) и сортируем по ключу
        std::vector<std::pair<int, int>> pairs;
        pairs.reserve(keys.size());
        for (size_t i = 0; i < keys.size(); ++i) {
            pairs.emplace_back(keys[i], weights[i]);
        }

        std::sort(pairs.begin(), pairs.end(), 
            [](const auto& a, const auto& b) {
                return a.first < b.first;  // по ключу, возрастание
            });

        // Сохраняем отсортированные данные
        keys_.resize(pairs.size());
        weights_.resize(pairs.size());
        for (size_t i = 0; i < pairs.size(); ++i) {
            keys_[i] = pairs[i].first;
            weights_[i] = pairs[i].second;
        }

        // Вычисляем префиксные суммы весов для быстрого поиска центра
        std::vector<int64_t> prefixSum(keys_.size() + 1, 0);
        for (size_t i = 0; i < weights_.size(); ++i) {
            prefixSum[i + 1] = prefixSum[i] + weights_[i];
        }

        // Рекурсивно строим дерево
        root_ = buildRecursive(0, static_cast<int>(keys_.size()) - 1, prefixSum, 1);
        size_ = keys_.size();
    }

private:
    /**
     * @brief Рекурсивное построение поддерева
     * @param left Левая граница (включительно)
     * @param right Правая граница (включительно)
     * @param prefixSum Префиксные суммы весов
     * @param depth Текущая глубина
     */
    std::unique_ptr<Node> buildRecursive(int left, int right, 
                                          const std::vector<int64_t>& prefixSum,
                                          int depth) {
        if (left > right) {
            return nullptr;
        }

        if (left == right) {
            return std::make_unique<Node>(keys_[left], weights_[left], depth);
        }

        // Находим центр тяжести
        int rootIdx = findCenterOfGravity(left, right, prefixSum);

        auto node = std::make_unique<Node>(keys_[rootIdx], weights_[rootIdx], depth);
        
        // Рекурсивно строим поддеревья
        node->left = buildRecursive(left, rootIdx - 1, prefixSum, depth + 1);
        node->right = buildRecursive(rootIdx + 1, right, prefixSum, depth + 1);

        return node;
    }

    /**
     * @brief Найти индекс "центра тяжести"
     * 
     * Ищем k, при котором |leftWeight - rightWeight| минимально,
     * где leftWeight = Σw[left..k-1], rightWeight = Σw[k+1..right]
     */
    int findCenterOfGravity(int left, int right, const std::vector<int64_t>& prefixSum) {
        // Вес всего сегмента [left..right]
        int64_t totalWeight = prefixSum[right + 1] - prefixSum[left];
        
        int bestIdx = left;
        int64_t minDiff = std::abs(totalWeight - 2 * weights_[left]);
        
        for (int k = left; k <= right; ++k) {
            // leftWeight = сумма весов [left..k-1]
            int64_t leftWeight = prefixSum[k] - prefixSum[left];
            
            // rightWeight = сумма весов [k+1..right]
            int64_t rightWeight = prefixSum[right + 1] - prefixSum[k + 1];
            
            int64_t diff = std::abs(leftWeight - rightWeight);
            
            if (diff < minDiff) {
                minDiff = diff;
                bestIdx = k;
            }
        }
        
        return bestIdx;
    }
};
