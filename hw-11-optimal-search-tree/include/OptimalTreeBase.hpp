#pragma once

#include "IBinaryTree.hpp"
#include <vector>
#include <stdexcept>
#include <memory>
#include <numeric>

/**
 * @brief Базовый класс для деревьев оптимального поиска (ДОП)
 * 
 * ДОП — статическое дерево, строится один раз из известных ключей и весов.
 * После построения insert()/remove() запрещены (нарушат оптимальность).
 * 
 * ВАЖНО: Осознанное нарушение SOLID (Liskov Substitution Principle).
 * В учебном проекте это допустимый компромисс для совместимости с IBinaryTree
 * в бенчмарках. В production-коде следует использовать отдельную иерархию
 * интерфейсов ISearchableTree → IDynamicTree / IStaticTree.
 */
class OptimalTreeBase : public IBinaryTree {
protected:
    /**
     * @brief Узел дерева
     */
    struct Node {
        int key;
        int weight;
        int depth;  // Глубина узла (корень = 1)
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

        Node(int k, int w, int d = 1) 
            : key(k), weight(w), depth(d), left(nullptr), right(nullptr) {}
    };

    std::unique_ptr<Node> root_;
    size_t size_;
    std::vector<int> keys_;
    std::vector<int> weights_;

public:
    OptimalTreeBase() : root_(nullptr), size_(0) {}
    ~OptimalTreeBase() override = default;

    // Запрет копирования
    OptimalTreeBase(const OptimalTreeBase&) = delete;
    OptimalTreeBase& operator=(const OptimalTreeBase&) = delete;

    // Разрешение перемещения
    OptimalTreeBase(OptimalTreeBase&&) = default;
    OptimalTreeBase& operator=(OptimalTreeBase&&) = default;

    // ========== Запрещённые динамические операции ==========
    
    /**
     * @brief Вставка запрещена для ДОП
     * @throws std::logic_error всегда
     */
    void insert(int x) final override {
        (void)x;
        throw std::logic_error("Optimal tree is static. Use build() instead of insert()");
    }

    /**
     * @brief Удаление запрещено для ДОП
     * @throws std::logic_error всегда
     */
    void remove(int x) final override {
        (void)x;
        throw std::logic_error("Optimal tree is static. Remove is not supported");
    }

    // ========== Разрешённые операции ==========

    bool search(int x) const override {
        return searchRecursive(root_.get(), x);
    }

    size_t size() const override { return size_; }
    bool empty() const override { return size_ == 0; }
    
    void clear() override {
        root_.reset();
        size_ = 0;
        keys_.clear();
        weights_.clear();
    }

    int height() const override {
        return heightRecursive(root_.get());
    }

    // ========== Специфичные для ДОП методы ==========

    /**
     * @brief Построить дерево из ключей и весов
     * @param keys Отсортированные ключи (или будут отсортированы внутри)
     * @param weights Веса ключей (частоты обращения)
     * @pre keys.size() == weights.size()
     */
    virtual void build(const std::vector<int>& keys, 
                       const std::vector<int>& weights) = 0;

    /**
     * @brief Получить средневзвешенную высоту дерева
     * @return hср = Σ(wi × hi) / W, где W = Σwi
     * 
     * Чем меньше hср, тем эффективнее дерево для данного распределения весов.
     */
    double getWeightedHeight() const {
        if (size_ == 0) return 0.0;
        
        int64_t totalWeight = std::accumulate(weights_.begin(), weights_.end(), 0LL);
        if (totalWeight == 0) return 0.0;
        
        int64_t weightedSum = calculateWeightedSum(root_.get());
        return static_cast<double>(weightedSum) / totalWeight;
    }

    /**
     * @brief Получить веса ключей
     */
    const std::vector<int>& getWeights() const { return weights_; }

    /**
     * @brief Получить ключи
     */
    const std::vector<int>& getKeys() const { return keys_; }

protected:
    bool searchRecursive(const Node* node, int x) const {
        if (!node) return false;
        
        if (x == node->key) return true;
        if (x < node->key) return searchRecursive(node->left.get(), x);
        return searchRecursive(node->right.get(), x);
    }

    int heightRecursive(const Node* node) const {
        if (!node) return 0;
        return 1 + std::max(heightRecursive(node->left.get()),
                           heightRecursive(node->right.get()));
    }

    /**
     * @brief Рекурсивный подсчёт Σ(wi × hi)
     */
    int64_t calculateWeightedSum(const Node* node) const {
        if (!node) return 0;
        
        int64_t sum = static_cast<int64_t>(node->weight) * node->depth;
        sum += calculateWeightedSum(node->left.get());
        sum += calculateWeightedSum(node->right.get());
        return sum;
    }

    /**
     * @brief Обновить глубину поддерева
     */
    void updateDepths(Node* node, int depth) {
        if (!node) return;
        node->depth = depth;
        updateDepths(node->left.get(), depth + 1);
        updateDepths(node->right.get(), depth + 1);
    }
};
