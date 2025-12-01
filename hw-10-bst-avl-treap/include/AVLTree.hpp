#pragma once

#include "IBinaryTree.hpp"
#include <memory>
#include <algorithm>

/**
 * @brief АВЛ-дерево (самобалансирующееся BST)
 * 
 * Инвариант: для каждого узла |height(left) - height(right)| <= 1
 * 
 * Сложность операций: O(log n) гарантированно
 */
class AVLTree : public IBinaryTree {
protected:
    /**
     * @brief Узел АВЛ-дерева с хранением высоты
     */
    struct Node {
        int key;
        int height;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

        explicit Node(int k) : key(k), height(1), left(nullptr), right(nullptr) {}
    };

    std::unique_ptr<Node> root_;
    size_t size_;

public:
    AVLTree();
    ~AVLTree() override = default;

    // Запрет копирования
    AVLTree(const AVLTree&) = delete;
    AVLTree& operator=(const AVLTree&) = delete;

    // Разрешение перемещения
    AVLTree(AVLTree&&) = default;
    AVLTree& operator=(AVLTree&&) = default;

    void insert(int x) override;
    bool search(int x) const override;
    void remove(int x) override;

    size_t size() const override { return size_; }
    bool empty() const override { return size_ == 0; }
    void clear() override;
    int height() const override;

    std::string getName() const override { return "AVLTree"; }

protected:
    /**
     * @brief Получить высоту узла (nullptr-safe)
     */
    int getHeight(const Node* node) const;

    /**
     * @brief Получить баланс-фактор узла
     * @return height(right) - height(left)
     */
    int getBalance(const Node* node) const;

    /**
     * @brief Обновить высоту узла на основе детей
     */
    void updateHeight(Node* node);

    /**
     * @brief Малое левое вращение
     */
    std::unique_ptr<Node> smallLeftRotation(std::unique_ptr<Node> a);

    /**
     * @brief Малое правое вращение
     */
    std::unique_ptr<Node> smallRightRotation(std::unique_ptr<Node> a);

    /**
     * @brief Большое левое вращение (правое + левое)
     */
    std::unique_ptr<Node> bigLeftRotation(std::unique_ptr<Node> a);

    /**
     * @brief Большое правое вращение (левое + правое)
     */
    std::unique_ptr<Node> bigRightRotation(std::unique_ptr<Node> a);

    /**
     * @brief Перебалансировка узла
     */
    std::unique_ptr<Node> rebalance(std::unique_ptr<Node> node);

    /**
     * @brief Рекурсивная вставка с балансировкой
     */
    std::unique_ptr<Node> insertRecursive(std::unique_ptr<Node> node, int x);

    /**
     * @brief Рекурсивный поиск
     */
    bool searchRecursive(const Node* node, int x) const;

    /**
     * @brief Рекурсивное удаление с балансировкой
     */
    std::unique_ptr<Node> removeRecursive(std::unique_ptr<Node> node, int x);

    /**
     * @brief Найти минимальный узел
     */
    Node* findMin(Node* node) const;
};