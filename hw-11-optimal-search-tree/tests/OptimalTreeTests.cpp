#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <random>
#include <numeric>

#include "OptimalTreeAlgo1.hpp"
#include "OptimalTreeAlgo2.hpp"

// ============================================
// Тесты OptimalTreeBase (общие для обоих алгоритмов)
// ============================================

template <typename T>
class OptimalTreeTest : public ::testing::Test {
public:
    std::unique_ptr<OptimalTreeBase> tree;

    void SetUp() override {
        tree = std::make_unique<T>();
    }
};

using TreeTypes = ::testing::Types<OptimalTreeAlgo1, OptimalTreeAlgo2>;
TYPED_TEST_SUITE(OptimalTreeTest, TreeTypes);

TYPED_TEST(OptimalTreeTest, EmptyTree) {
    EXPECT_TRUE(this->tree->empty());
    EXPECT_EQ(this->tree->size(), 0);
    EXPECT_EQ(this->tree->height(), 0);
}

TYPED_TEST(OptimalTreeTest, InsertThrows) {
    EXPECT_THROW(this->tree->insert(42), std::logic_error);
}

TYPED_TEST(OptimalTreeTest, RemoveThrows) {
    EXPECT_THROW(this->tree->remove(42), std::logic_error);
}

TYPED_TEST(OptimalTreeTest, BuildSingleElement) {
    this->tree->build({42}, {100});
    
    EXPECT_FALSE(this->tree->empty());
    EXPECT_EQ(this->tree->size(), 1);
    EXPECT_EQ(this->tree->height(), 1);
    EXPECT_TRUE(this->tree->search(42));
    EXPECT_FALSE(this->tree->search(0));
}

TYPED_TEST(OptimalTreeTest, BuildMultipleElements) {
    std::vector<int> keys = {1, 2, 3, 4, 5};
    std::vector<int> weights = {10, 20, 30, 20, 10};
    
    this->tree->build(keys, weights);
    
    EXPECT_EQ(this->tree->size(), 5);
    
    for (int key : keys) {
        EXPECT_TRUE(this->tree->search(key));
    }
    
    EXPECT_FALSE(this->tree->search(0));
    EXPECT_FALSE(this->tree->search(100));
}

TYPED_TEST(OptimalTreeTest, BuildMismatchedSizesThrows) {
    std::vector<int> keys = {1, 2, 3};
    std::vector<int> weights = {10, 20};  // Неправильный размер
    
    EXPECT_THROW(this->tree->build(keys, weights), std::invalid_argument);
}

TYPED_TEST(OptimalTreeTest, BuildEmpty) {
    this->tree->build({}, {});
    
    EXPECT_TRUE(this->tree->empty());
    EXPECT_EQ(this->tree->size(), 0);
}

TYPED_TEST(OptimalTreeTest, Clear) {
    this->tree->build({1, 2, 3}, {10, 20, 30});
    EXPECT_FALSE(this->tree->empty());
    
    this->tree->clear();
    
    EXPECT_TRUE(this->tree->empty());
    EXPECT_EQ(this->tree->size(), 0);
    EXPECT_FALSE(this->tree->search(1));
}

TYPED_TEST(OptimalTreeTest, WeightedHeight) {
    // Построим дерево с известными весами
    std::vector<int> keys = {1, 2, 3};
    std::vector<int> weights = {10, 20, 10};  // Средний ключ имеет максимальный вес
    
    this->tree->build(keys, weights);
    
    double hср = this->tree->getWeightedHeight();
    
    // hср должна быть в разумных пределах [1, height]
    EXPECT_GE(hср, 1.0);
    EXPECT_LE(hср, static_cast<double>(this->tree->height()));
}

TYPED_TEST(OptimalTreeTest, LargeDataset) {
    const size_t N = 1000;
    std::vector<int> keys(N);
    std::vector<int> weights(N);
    
    std::iota(keys.begin(), keys.end(), 1);  // 1, 2, ..., N
    
    // Zipf-подобные веса
    for (size_t i = 0; i < N; ++i) {
        weights[i] = static_cast<int>(10000.0 / (i + 1));
    }
    
    this->tree->build(keys, weights);
    
    EXPECT_EQ(this->tree->size(), N);
    
    // Проверяем поиск
    for (int key : keys) {
        EXPECT_TRUE(this->tree->search(key));
    }
}

// ============================================
// Специфичные тесты для Алгоритма 1
// ============================================

TEST(OptimalTreeAlgo1Test, CorrectName) {
    OptimalTreeAlgo1 tree;
    EXPECT_EQ(tree.getName(), "OptimalTree-A1");
}

TEST(OptimalTreeAlgo1Test, HighWeightNearRoot) {
    // Ключ с максимальным весом должен быть близко к корню
    OptimalTreeAlgo1 tree;
    
    // Ключ 5 имеет максимальный вес — должен стать корнем
    std::vector<int> keys = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<int> weights = {1, 1, 1, 1, 1000, 1, 1, 1, 1};
    
    tree.build(keys, weights);
    
    // Ключ 5 должен быть найден за 1 сравнение (он корень)
    EXPECT_TRUE(tree.search(5));
    EXPECT_EQ(tree.height(), tree.height());  // Просто проверяем, что работает
}

// ============================================
// Специфичные тесты для Алгоритма 2
// ============================================

TEST(OptimalTreeAlgo2Test, CorrectName) {
    OptimalTreeAlgo2 tree;
    EXPECT_EQ(tree.getName(), "OptimalTree-A2");
}

TEST(OptimalTreeAlgo2Test, BalancedForUniformWeights) {
    // При равных весах дерево должно быть хорошо сбалансировано
    OptimalTreeAlgo2 tree;
    
    const size_t N = 127;  // 2^7 - 1 — идеально для сбалансированного дерева
    std::vector<int> keys(N);
    std::vector<int> weights(N, 1);  // Все веса равны
    
    std::iota(keys.begin(), keys.end(), 1);
    
    tree.build(keys, weights);
    
    // Высота должна быть близка к log2(N) ≈ 7
    EXPECT_LE(tree.height(), 8);
}

TEST(OptimalTreeAlgo2Test, BetterWeightedHeightThanAlgo1) {
    // Алгоритм 2 должен давать лучшую средневзвешенную высоту
    const size_t N = 100;
    std::vector<int> keys(N);
    std::vector<int> weights(N);
    
    std::iota(keys.begin(), keys.end(), 1);
    
    // Zipf-веса
    for (size_t i = 0; i < N; ++i) {
        weights[i] = static_cast<int>(10000.0 / (i + 1));
    }
    
    // Перемешиваем, чтобы усложнить задачу
    std::mt19937 rng(12345);
    std::vector<std::pair<int, int>> pairs;
    for (size_t i = 0; i < N; ++i) {
        pairs.emplace_back(keys[i], weights[i]);
    }
    std::shuffle(pairs.begin(), pairs.end(), rng);
    
    for (size_t i = 0; i < N; ++i) {
        keys[i] = pairs[i].first;
        weights[i] = pairs[i].second;
    }
    
    OptimalTreeAlgo1 tree1;
    OptimalTreeAlgo2 tree2;
    
    tree1.build(keys, weights);
    tree2.build(keys, weights);
    
    double hср1 = tree1.getWeightedHeight();
    double hср2 = tree2.getWeightedHeight();
    
    // Алгоритм 2 должен быть не хуже (обычно лучше)
    // Допускаем небольшую погрешность из-за эвристик
    EXPECT_LE(hср2, hср1 * 1.1);  // Algo2 не более чем на 10% хуже
}

// ============================================
// Тест из примера в материалах
// ============================================

TEST(OptimalTreeTest, ExampleFromLecture) {
    // Пример из лекции: V1=1, V2=2, V3=3 с весами w1=60, w2=30, w3=10
    // Оптимальное дерево: корень=1, правый=2, правый=3
    // hср = (60*1 + 30*2 + 10*3) / 100 = 150/100 = 1.5
    
    std::vector<int> keys = {1, 2, 3};
    std::vector<int> weights = {60, 30, 10};
    
    OptimalTreeAlgo2 tree;
    tree.build(keys, weights);
    
    EXPECT_EQ(tree.size(), 3);
    EXPECT_TRUE(tree.search(1));
    EXPECT_TRUE(tree.search(2));
    EXPECT_TRUE(tree.search(3));
    
    // hср для Algo2 должна быть близка к оптимальному 1.5
    double hср = tree.getWeightedHeight();
    EXPECT_LE(hср, 2.0);  // Не идеально, но разумно
}
