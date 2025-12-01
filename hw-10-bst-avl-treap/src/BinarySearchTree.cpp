#include "BinarySearchTree.hpp"

BinarySearchTree::BinarySearchTree() : root_(nullptr), size_(0) {}

void BinarySearchTree::insert(int x) {
    root_ = insertRecursive(std::move(root_), x);
}

std::unique_ptr<BinarySearchTree::Node> 
BinarySearchTree::insertRecursive(std::unique_ptr<Node> node, int x) {
    if (!node) {
        ++size_;
        return std::make_unique<Node>(x);
    }

    if (x < node->key) {
        node->left = insertRecursive(std::move(node->left), x);
    } else if (x > node->key) {
        node->right = insertRecursive(std::move(node->right), x);
    }
    // Если x == node->key, элемент уже существует — не вставляем

    return node;
}

bool BinarySearchTree::search(int x) const {
    return searchRecursive(root_.get(), x);
}

bool BinarySearchTree::searchRecursive(const Node* node, int x) const {
    if (!node) {
        return false;
    }

    if (x == node->key) {
        return true;
    } else if (x < node->key) {
        return searchRecursive(node->left.get(), x);
    } else {
        return searchRecursive(node->right.get(), x);
    }
}

void BinarySearchTree::remove(int x) {
    root_ = removeRecursive(std::move(root_), x);
}

std::unique_ptr<BinarySearchTree::Node> 
BinarySearchTree::removeRecursive(std::unique_ptr<Node> node, int x) {
    if (!node) {
        return nullptr;
    }

    if (x < node->key) {
        node->left = removeRecursive(std::move(node->left), x);
    } else if (x > node->key) {
        node->right = removeRecursive(std::move(node->right), x);
    } else {
        // Нашли узел для удаления
        --size_;

        // Случай 1: Нет левого потомка
        if (!node->left) {
            return std::move(node->right);
        }
        // Случай 2: Нет правого потомка
        if (!node->right) {
            return std::move(node->left);
        }
        // Случай 3: Два потомка
        // Находим минимум в правом поддереве
        Node* minNode = findMin(node->right.get());
        node->key = minNode->key;
        ++size_; // Компенсируем, т.к. removeRecursive уменьшит size_
        node->right = removeRecursive(std::move(node->right), minNode->key);
    }

    return node;
}

BinarySearchTree::Node* BinarySearchTree::findMin(Node* node) const {
    while (node->left) {
        node = node->left.get();
    }
    return node;
}

void BinarySearchTree::clear() {
    root_.reset();
    size_ = 0;
}

int BinarySearchTree::height() const {
    return heightRecursive(root_.get());
}

int BinarySearchTree::heightRecursive(const Node* node) const {
    if (!node) {
        return 0;
    }
    return 1 + std::max(heightRecursive(node->left.get()), 
                        heightRecursive(node->right.get()));
}

void BinarySearchTree::inorderTraversal(std::function<void(int)> callback) const {
    inorderRecursive(root_.get(), callback);
}

void BinarySearchTree::inorderRecursive(const Node* node, 
                                         std::function<void(int)>& callback) const {
    if (!node) return;
    inorderRecursive(node->left.get(), callback);
    callback(node->key);
    inorderRecursive(node->right.get(), callback);
}