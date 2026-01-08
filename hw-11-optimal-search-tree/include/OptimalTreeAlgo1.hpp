#pragma once

#include "OptimalTreeBase.hpp"
#include <algorithm>
#include <vector>

/**
 * @brief Алгоритм 1 построения приближённого ДОП
 * 
 * Идея: Вставлять ключи в порядке убывания весов в обычное BST.
 * Ключи с большим весом окажутся ближе к корню.
 * 
 * Алгоритм:
 * 1. Отсортировать пары (ключ, вес) по весу (убывание)
 * 2. Вставлять ключи в BST в этом порядке
 * 
 * Сложность: O(n log n) сортировка + O(n²) вставка в BST = O(n²) в худшем случае
 * 
 * ВАЖНО: Этот алгоритм считается "плохим" — результат близок к случайному дереву.
 * Включён для сравнения с Алгоритмом 2.
 */
class OptimalTreeAlgo1 : public OptimalTreeBase {
public:
    OptimalTreeAlgo1() = default;
    ~OptimalTreeAlgo1() override = default;

    std::string getName() const override { return "OptimalTree-A1"; }

    /**
     * @brief Построить дерево: сортировка по весам → вставка в BST
     */
    void build(const std::vector<int>& keys, 
               const std::vector<int>& weights) override {
        if (keys.size() != weights.size()) {
            throw std::invalid_argument("Keys and weights must have same size");
        }
        if (keys.empty()) return;

        clear();
        keys_ = keys;
        weights_ = weights;

        // Создаём пары (ключ, вес, оригинальный индекс)
        std::vector<std::tuple<int, int, size_t>> pairs;
        pairs.reserve(keys.size());
        for (size_t i = 0; i < keys.size(); ++i) {
            pairs.emplace_back(keys[i], weights[i], i);
        }

        // Сортируем по весу (убывание)
        std::sort(pairs.begin(), pairs.end(), 
            [](const auto& a, const auto& b) {
                return std::get<1>(a) > std::get<1>(b);  // по весу, убывание
            });

        // Вставляем в BST в порядке убывания весов
        for (const auto& [key, weight, idx] : pairs) {
            root_ = insertBST(std::move(root_), key, weight);
            ++size_;
        }

        // Обновляем глубины для корректного расчёта hср
        updateDepths(root_.get(), 1);
    }

private:
    /**
     * @brief Вставка в BST (стандартная)
     */
    std::unique_ptr<Node> insertBST(std::unique_ptr<Node> node, int key, int weight) {
        if (!node) {
            return std::make_unique<Node>(key, weight);
        }

        if (key < node->key) {
            node->left = insertBST(std::move(node->left), key, weight);
        } else if (key > node->key) {
            node->right = insertBST(std::move(node->right), key, weight);
        }
        // Дубликаты игнорируем

        return node;
    }
};
