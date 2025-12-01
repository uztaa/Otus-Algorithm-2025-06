#include "AVLTree.hpp"

AVLTree::AVLTree() : root_(nullptr), size_(0) {}

int AVLTree::getHeight(const Node* node) const {
    return node ? node->height : 0;
}

int AVLTree::getBalance(const Node* node) const {
    return node ? getHeight(node->right.get()) - getHeight(node->left.get()) : 0;
}

void AVLTree::updateHeight(Node* node) {
    if (node) {
        node->height = 1 + std::max(getHeight(node->left.get()), 
                                     getHeight(node->right.get()));
    }
}

std::unique_ptr<AVLTree::Node> AVLTree::smallLeftRotation(std::unique_ptr<Node> a) {
    //     a              b
    //      \            / \
    //       b    →     a   c
    //        \
    //         c
    std::unique_ptr<Node> b = std::move(a->right);
    a->right = std::move(b->left);
    updateHeight(a.get());
    b->left = std::move(a);
    updateHeight(b.get());
    return b;
}

std::unique_ptr<AVLTree::Node> AVLTree::smallRightRotation(std::unique_ptr<Node> a) {
    //       a          b
    //      /          / \
    //     b     →    c   a
    //    /
    //   c
    std::unique_ptr<Node> b = std::move(a->left);
    a->left = std::move(b->right);
    updateHeight(a.get());
    b->right = std::move(a);
    updateHeight(b.get());
    return b;
}

std::unique_ptr<AVLTree::Node> AVLTree::bigLeftRotation(std::unique_ptr<Node> a) {
    // Большое левое = малое правое для правого потомка + малое левое
    a->right = smallRightRotation(std::move(a->right));
    return smallLeftRotation(std::move(a));
}

std::unique_ptr<AVLTree::Node> AVLTree::bigRightRotation(std::unique_ptr<Node> a) {
    // Большое правое = малое левое для левого потомка + малое правое
    a->left = smallLeftRotation(std::move(a->left));
    return smallRightRotation(std::move(a));
}

std::unique_ptr<AVLTree::Node> AVLTree::rebalance(std::unique_ptr<Node> node) {
    updateHeight(node.get());
    int balance = getBalance(node.get());

    // Левое поддерево перевешивает (balance == -2)
    if (balance == -2) {
        if (getBalance(node->left.get()) > 0) {
            // Большое правое вращение
            return bigRightRotation(std::move(node));
        } else {
            // Малое правое вращение
            return smallRightRotation(std::move(node));
        }
    }

    // Правое поддерево перевешивает (balance == +2)
    if (balance == 2) {
        if (getBalance(node->right.get()) < 0) {
            // Большое левое вращение
            return bigLeftRotation(std::move(node));
        } else {
            // Малое левое вращение
            return smallLeftRotation(std::move(node));
        }
    }

    return node;
}

void AVLTree::insert(int x) {
    root_ = insertRecursive(std::move(root_), x);
}

std::unique_ptr<AVLTree::Node> 
AVLTree::insertRecursive(std::unique_ptr<Node> node, int x) {
    if (!node) {
        ++size_;
        return std::make_unique<Node>(x);
    }

    if (x < node->key) {
        node->left = insertRecursive(std::move(node->left), x);
    } else if (x > node->key) {
        node->right = insertRecursive(std::move(node->right), x);
    } else {
        // Дубликат — не вставляем
        return node;
    }

    return rebalance(std::move(node));
}

bool AVLTree::search(int x) const {
    return searchRecursive(root_.get(), x);
}

bool AVLTree::searchRecursive(const Node* node, int x) const {
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

void AVLTree::remove(int x) {
    root_ = removeRecursive(std::move(root_), x);
}

std::unique_ptr<AVLTree::Node> 
AVLTree::removeRecursive(std::unique_ptr<Node> node, int x) {
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

        if (!node->left) {
            return std::move(node->right);
        }
        if (!node->right) {
            return std::move(node->left);
        }

        // Два потомка: заменяем минимумом из правого поддерева
        Node* minNode = findMin(node->right.get());
        node->key = minNode->key;
        ++size_;
        node->right = removeRecursive(std::move(node->right), minNode->key);
    }

    return rebalance(std::move(node));
}

AVLTree::Node* AVLTree::findMin(Node* node) const {
    while (node->left) {
        node = node->left.get();
    }
    return node;
}

void AVLTree::clear() {
    root_.reset();
    size_ = 0;
}

int AVLTree::height() const {
    return getHeight(root_.get());
}