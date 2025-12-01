#pragma once

#include "IBinaryTree.hpp"
#include <memory>
#include <functional>

/**
 * @brief Простое двоичное дерево поиска (BST)
 * 
 * Свойство BST: для каждого узла все ключи в левом поддереве меньше,
 * все ключи в правом поддереве больше.
 * 
 * Сложность операций:
 * - Средний случай: O(log n)
 * - Худший случай: O(n) (вырожденное дерево)
 */
class BinarySearchTree : public IBinaryTree {
protected:
    /**
     * @brief Узел дерева
     */
    struct Node {
        int key;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

        explicit Node(int k) : key(k), left(nullptr), right(nullptr) {}
    };

    std::unique_ptr<Node> root_;
    size_t size_;

public:
    BinarySearchTree();
    ~BinarySearchTree() override = default;

    // Запрет копирования
    BinarySearchTree(const BinarySearchTree&) = delete;
    BinarySearchTree& operator=(const BinarySearchTree&) = delete;

    // Разрешение перемещения
    BinarySearchTree(BinarySearchTree&&) = default;
    BinarySearchTree& operator=(BinarySearchTree&&) = default;

    void insert(int x) override;
    bool search(int x) const override;
    void remove(int x) override;

    size_t size() const override { return size_; }
    bool empty() const override { return size_ == 0; }
    void clear() override;
    int height() const override;

    std::string getName() const override { return "BinarySearchTree"; }

    /**
     * @brief Обход дерева in-order (для отладки)
     * @param callback Функция, вызываемая для каждого узла
     */
    void inorderTraversal(std::function<void(int)> callback) const;

protected:
    /**
     * @brief Рекурсивная вставка
     */
    std::unique_ptr<Node> insertRecursive(std::unique_ptr<Node> node, int x);

    /**
     * @brief Рекурсивный поиск
     */
    bool searchRecursive(const Node* node, int x) const;

    /**
     * @brief Рекурсивное удаление
     */
    std::unique_ptr<Node> removeRecursive(std::unique_ptr<Node> node, int x);

    /**
     * @brief Найти минимальный узел в поддереве
     */
    Node* findMin(Node* node) const;

    /**
     * @brief Рекурсивное вычисление высоты
     */
    int heightRecursive(const Node* node) const;

    /**
     * @brief Рекурсивный обход in-order
     */
    void inorderRecursive(const Node* node, std::function<void(int)>& callback) const;
};