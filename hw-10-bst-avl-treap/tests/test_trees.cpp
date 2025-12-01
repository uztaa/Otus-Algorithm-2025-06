#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <random>
#include <memory>

#include "IBinaryTree.hpp"
#include "BinarySearchTree.hpp"
#include "AVLTree.hpp"
#include "Treap.hpp"

/**
 * @brief Typed test suite для всех деревьев
 */
template <typename T>
class BinaryTreeTest : public ::testing::Test {
public:
    std::unique_ptr<IBinaryTree> tree;

    void SetUp() override {
        tree = std::make_unique<T>();
    }
};

using TreeTypes = ::testing::Types<BinarySearchTree, AVLTree, Treap>;

TYPED_TEST_SUITE(BinaryTreeTest, TreeTypes);

// ============ Базовые тесты ============

TYPED_TEST(BinaryTreeTest, EmptyTree) {
    EXPECT_TRUE(this->tree->empty());
    EXPECT_EQ(this->tree->size(), 0);
    EXPECT_EQ(this->tree->height(), 0);
}

TYPED_TEST(BinaryTreeTest, InsertSingle) {
    this->tree->insert(42);
    EXPECT_FALSE(this->tree->empty());
    EXPECT_EQ(this->tree->size(), 1);
    EXPECT_TRUE(this->tree->search(42));
    EXPECT_FALSE(this->tree->search(0));
}

TYPED_TEST(BinaryTreeTest, InsertMultiple) {
    this->tree->insert(5);
    this->tree->insert(3);
    this->tree->insert(7);
    this->tree->insert(1);
    this->tree->insert(9);

    EXPECT_EQ(this->tree->size(), 5);
    EXPECT_TRUE(this->tree->search(5));
    EXPECT_TRUE(this->tree->search(3));
    EXPECT_TRUE(this->tree->search(7));
    EXPECT_TRUE(this->tree->search(1));
    EXPECT_TRUE(this->tree->search(9));
    EXPECT_FALSE(this->tree->search(0));
    EXPECT_FALSE(this->tree->search(100));
}

TYPED_TEST(BinaryTreeTest, InsertDuplicates) {
    this->tree->insert(5);
    this->tree->insert(5);
    this->tree->insert(5);

    EXPECT_EQ(this->tree->size(), 1);
    EXPECT_TRUE(this->tree->search(5));
}

TYPED_TEST(BinaryTreeTest, RemoveLeaf) {
    this->tree->insert(5);
    this->tree->insert(3);
    this->tree->insert(7);

    this->tree->remove(3);
    EXPECT_EQ(this->tree->size(), 2);
    EXPECT_FALSE(this->tree->search(3));
    EXPECT_TRUE(this->tree->search(5));
    EXPECT_TRUE(this->tree->search(7));
}

TYPED_TEST(BinaryTreeTest, RemoveNodeWithOneChild) {
    this->tree->insert(5);
    this->tree->insert(3);
    this->tree->insert(7);
    this->tree->insert(6);

    this->tree->remove(7);
    EXPECT_EQ(this->tree->size(), 3);
    EXPECT_FALSE(this->tree->search(7));
    EXPECT_TRUE(this->tree->search(6));
}

TYPED_TEST(BinaryTreeTest, RemoveNodeWithTwoChildren) {
    this->tree->insert(5);
    this->tree->insert(3);
    this->tree->insert(7);
    this->tree->insert(6);
    this->tree->insert(8);

    this->tree->remove(7);
    EXPECT_EQ(this->tree->size(), 4);
    EXPECT_FALSE(this->tree->search(7));
    EXPECT_TRUE(this->tree->search(6));
    EXPECT_TRUE(this->tree->search(8));
}

TYPED_TEST(BinaryTreeTest, RemoveRoot) {
    this->tree->insert(5);
    this->tree->insert(3);
    this->tree->insert(7);

    this->tree->remove(5);
    EXPECT_EQ(this->tree->size(), 2);
    EXPECT_FALSE(this->tree->search(5));
    EXPECT_TRUE(this->tree->search(3));
    EXPECT_TRUE(this->tree->search(7));
}

TYPED_TEST(BinaryTreeTest, RemoveNonexistent) {
    this->tree->insert(5);
    this->tree->insert(3);
    this->tree->insert(7);

    this->tree->remove(100);
    EXPECT_EQ(this->tree->size(), 3);
}

TYPED_TEST(BinaryTreeTest, Clear) {
    this->tree->insert(5);
    this->tree->insert(3);
    this->tree->insert(7);

    this->tree->clear();
    EXPECT_TRUE(this->tree->empty());
    EXPECT_EQ(this->tree->size(), 0);
    EXPECT_FALSE(this->tree->search(5));
}

TYPED_TEST(BinaryTreeTest, RandomOperations) {
    std::mt19937 rng(12345);
    std::uniform_int_distribution<int> dist(1, 1000);
    std::vector<int> inserted;

    // Вставка 100 случайных чисел
    for (int i = 0; i < 100; ++i) {
        int val = dist(rng);
        this->tree->insert(val);
        inserted.push_back(val);
    }

    // Проверка поиска
    for (int val : inserted) {
        EXPECT_TRUE(this->tree->search(val));
    }

    // Удаление половины
    std::shuffle(inserted.begin(), inserted.end(), rng);
    for (size_t i = 0; i < inserted.size() / 2; ++i) {
        this->tree->remove(inserted[i]);
    }

    // Проверка
    for (size_t i = 0; i < inserted.size() / 2; ++i) {
        EXPECT_FALSE(this->tree->search(inserted[i]));
    }
}

TYPED_TEST(BinaryTreeTest, SortedInsert) {
    // Вставка в возрастающем порядке
    for (int i = 1; i <= 100; ++i) {
        this->tree->insert(i);
    }

    EXPECT_EQ(this->tree->size(), 100);

    for (int i = 1; i <= 100; ++i) {
        EXPECT_TRUE(this->tree->search(i));
    }
}

// ============ Специфичные тесты для AVL ============

TEST(AVLTreeTest, BalanceAfterInsert) {
    AVLTree tree;

    // Вставляем в возрастающем порядке — должно балансироваться
    for (int i = 1; i <= 15; ++i) {
        tree.insert(i);
    }

    // Высота сбалансированного дерева ~log2(15) ≈ 4
    EXPECT_LE(tree.height(), 5);
}

TEST(AVLTreeTest, BalanceAfterRemove) {
    AVLTree tree;

    for (int i = 1; i <= 31; ++i) {
        tree.insert(i);
    }

    // Удаляем половину
    for (int i = 1; i <= 15; ++i) {
        tree.remove(i * 2);
    }

    // Проверяем, что дерево всё ещё сбалансировано
    EXPECT_LE(tree.height(), 5);
}

// ============ Специфичные тесты для Treap ============

TEST(TreapTest, DeterministicSeed) {
    Treap tree1(42);
    Treap tree2(42);

    for (int i = 1; i <= 10; ++i) {
        tree1.insert(i);
        tree2.insert(i);
    }

    // При одинаковом seed высоты должны совпадать
    EXPECT_EQ(tree1.height(), tree2.height());
}

// ============ Индивидуальные тесты имён ============

TEST(BinarySearchTreeTest, CorrectName) {
    BinarySearchTree tree;
    EXPECT_EQ(tree.getName(), "BinarySearchTree");
}

TEST(AVLTreeTest, CorrectName) {
    AVLTree tree;
    EXPECT_EQ(tree.getName(), "AVLTree");
}

TEST(TreapTest, CorrectName) {
    Treap tree;
    EXPECT_EQ(tree.getName(), "Treap");
}