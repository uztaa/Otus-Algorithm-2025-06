#include "Treap.hpp"

Treap::Treap() : root_(nullptr), size_(0), rng_(std::random_device{}()) {}

Treap::Treap(uint32_t seed) : root_(nullptr), size_(0), rng_(seed) {}

int Treap::generatePriority() {
    return std::uniform_int_distribution<int>()(rng_);
}

std::pair<std::unique_ptr<Treap::Node>, std::unique_ptr<Treap::Node>>
Treap::split(std::unique_ptr<Node> t, int x) {
    if (!t) {
        return {nullptr, nullptr};
    }

    if (t->key < x) {
        // Текущий узел идёт в левое дерево
        auto [leftPart, rightPart] = split(std::move(t->right), x);
        t->right = std::move(leftPart);
        return {std::move(t), std::move(rightPart)};
    } else {
        // Текущий узел идёт в правое дерево
        auto [leftPart, rightPart] = split(std::move(t->left), x);
        t->left = std::move(rightPart);
        return {std::move(leftPart), std::move(t)};
    }
}

std::unique_ptr<Treap::Node> 
Treap::merge(std::unique_ptr<Node> l, std::unique_ptr<Node> r) {
    if (!l) return r;
    if (!r) return l;

    if (l->priority > r->priority) {
        // l становится корнем
        l->right = merge(std::move(l->right), std::move(r));
        return l;
    } else {
        // r становится корнем
        r->left = merge(std::move(l), std::move(r->left));
        return r;
    }
}

void Treap::insert(int x) {
    // Проверяем, есть ли уже такой элемент
    if (search(x)) {
        return;
    }

    auto [l, r] = split(std::move(root_), x);
    auto newNode = std::make_unique<Node>(x, generatePriority());
    root_ = merge(merge(std::move(l), std::move(newNode)), std::move(r));
    ++size_;
}

bool Treap::search(int x) const {
    return searchRecursive(root_.get(), x);
}

bool Treap::searchRecursive(const Node* node, int x) const {
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

void Treap::remove(int x) {
    if (!search(x)) {
        return;
    }

    auto [l, temp] = split(std::move(root_), x);
    auto [mid, r] = split(std::move(temp), x + 1);
    // mid содержит узел с ключом x — отбрасываем его
    root_ = merge(std::move(l), std::move(r));
    --size_;
}

void Treap::clear() {
    root_.reset();
    size_ = 0;
}

int Treap::height() const {
    return heightRecursive(root_.get());
}

int Treap::heightRecursive(const Node* node) const {
    if (!node) {
        return 0;
    }
    return 1 + std::max(heightRecursive(node->left.get()), 
                        heightRecursive(node->right.get()));
}