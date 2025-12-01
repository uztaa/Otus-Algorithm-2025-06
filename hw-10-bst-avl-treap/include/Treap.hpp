#pragma once

#include "IBinaryTree.hpp"
#include <memory>
#include <random>
#include <utility>

/**
 * @brief Декартово дерево (Treap = Tree + Heap)
 * 
 * Комбинирует свойства BST (по ключу) и кучи (по приоритету).
 * При случайных приоритетах обеспечивает O(log n) в среднем.
 * 
 * Ключевые операции: split и merge
 */
class Treap : public IBinaryTree {
protected:
    /**
     * @brief Узел декартова дерева
     */
    struct Node {
        int key;
        int priority;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

        Node(int k, int p) : key(k), priority(p), left(nullptr), right(nullptr) {}
    };

    std::unique_ptr<Node> root_;
    size_t size_;
    mutable std::mt19937 rng_;

public:
    Treap();
    explicit Treap(uint32_t seed);
    ~Treap() override = default;

    // Запрет копирования
    Treap(const Treap&) = delete;
    Treap& operator=(const Treap&) = delete;

    // Разрешение перемещения
    Treap(Treap&&) = default;
    Treap& operator=(Treap&&) = default;

    void insert(int x) override;
    bool search(int x) const override;
    void remove(int x) override;

    size_t size() const override { return size_; }
    bool empty() const override { return size_ == 0; }
    void clear() override;
    int height() const override;

    std::string getName() const override { return "Treap"; }

protected:
    /**
     * @brief Генерация случайного приоритета
     */
    int generatePriority();

    /**
     * @brief Разделение дерева по ключу x
     * @param t Исходное дерево
     * @param x Ключ разделения
     * @return Пара (L, R), где все ключи в L < x, все ключи в R >= x
     */
    std::pair<std::unique_ptr<Node>, std::unique_ptr<Node>> 
    split(std::unique_ptr<Node> t, int x);

    /**
     * @brief Объединение двух деревьев
     * @param l Левое дерево (все ключи меньше)
     * @param r Правое дерево (все ключи больше)
     * @return Объединённое дерево
     * @pre Все ключи в l < всех ключей в r
     */
    std::unique_ptr<Node> merge(std::unique_ptr<Node> l, std::unique_ptr<Node> r);

    /**
     * @brief Рекурсивный поиск
     */
    bool searchRecursive(const Node* node, int x) const;

    /**
     * @brief Рекурсивное вычисление высоты
     */
    int heightRecursive(const Node* node) const;
};