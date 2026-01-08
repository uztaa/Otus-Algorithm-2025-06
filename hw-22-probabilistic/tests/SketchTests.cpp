#include <gtest/gtest.h>
#include <unordered_set>
#include <unordered_map>
#include <random>
#include <string>
#include <cmath>

#include "sketch/BloomFilter.hpp"
#include "sketch/HyperLogLog.hpp"
#include "sketch/CountMinSketch.hpp"
#include "sketch/MurmurHasher.hpp"

// ============================================
// Bloom Filter Tests
// ============================================

class BloomFilterTest : public ::testing::Test {
protected:
    void SetUp() override {
        bf = std::make_unique<BloomFilter>(10000, 0.01);
    }
    
    std::unique_ptr<BloomFilter> bf;
};

TEST_F(BloomFilterTest, AddAndContains) {
    bf->add(std::string("hello"));
    bf->add(std::string("world"));
    
    EXPECT_TRUE(bf->contains(std::string("hello")));
    EXPECT_TRUE(bf->contains(std::string("world")));
}

TEST_F(BloomFilterTest, AddAndContainsUint64) {
    bf->add(uint64_t(12345));
    bf->add(uint64_t(67890));
    
    EXPECT_TRUE(bf->contains(uint64_t(12345)));
    EXPECT_TRUE(bf->contains(uint64_t(67890)));
}

TEST_F(BloomFilterTest, Clear) {
    bf->add(std::string("hello"));
    EXPECT_TRUE(bf->contains(std::string("hello")));
    
    bf->clear();
    
    EXPECT_FALSE(bf->contains(std::string("hello")));
}

TEST_F(BloomFilterTest, NoFalseNegatives) {
    std::vector<std::string> elements;
    for (int i = 0; i < 1000; ++i) {
        elements.push_back("element_" + std::to_string(i));
        bf->add(elements.back());
    }
    
    for (const auto& elem : elements) {
        EXPECT_TRUE(bf->contains(elem)) << "False Negative for: " << elem;
    }
}

TEST_F(BloomFilterTest, FalsePositiveRate) {
    const int n = 5000;
    for (int i = 0; i < n; ++i) {
        bf->add(uint64_t(i));
    }
    
    int falsePositives = 0;
    const int tests = 10000;
    for (int i = n; i < n + tests; ++i) {
        if (bf->contains(uint64_t(i))) {
            ++falsePositives;
        }
    }
    
    double fpr = static_cast<double>(falsePositives) / tests;
    EXPECT_LT(fpr, 0.05) << "False Positive Rate too high: " << fpr;
}

TEST_F(BloomFilterTest, GetMemoryUsage) {
    EXPECT_GT(bf->getMemoryUsage(), 0u);
}

TEST_F(BloomFilterTest, GetName) {
    EXPECT_EQ(bf->getName(), "BloomFilter");
}

TEST_F(BloomFilterTest, InvalidParameters) {
    EXPECT_THROW(BloomFilter(0, 0.01), std::invalid_argument);
    EXPECT_THROW(BloomFilter(1000, 0.0), std::invalid_argument);
    EXPECT_THROW(BloomFilter(1000, 1.0), std::invalid_argument);
    EXPECT_THROW(BloomFilter(1000, -0.1), std::invalid_argument);
}

TEST_F(BloomFilterTest, ExplicitConstructorWithHasher) {
    // Используем явный конструктор с hasher для избежания неоднозначности
    // BloomFilter(size_t, size_t, HasherPtr) — явные размеры
    auto hasher = std::make_shared<StdHasher>();
    BloomFilter bf2(size_t(1000), size_t(7), hasher);
    EXPECT_EQ(bf2.getNumBits(), 1000u);
    EXPECT_EQ(bf2.getNumHashes(), 7u);
}

TEST_F(BloomFilterTest, WithMurmurHasher) {
    auto hasher = std::make_shared<MurmurHasher>();
    BloomFilter bf2(10000, 0.01, hasher);
    
    bf2.add(std::string("test"));
    EXPECT_TRUE(bf2.contains(std::string("test")));
    EXPECT_EQ(bf2.getHasherName(), "MurmurHash3");
}

// ============================================
// HyperLogLog Tests
// ============================================

class HyperLogLogTest : public ::testing::Test {
protected:
    void SetUp() override {
        hll = std::make_unique<HyperLogLog>(14);
    }
    
    std::unique_ptr<HyperLogLog> hll;
};

TEST_F(HyperLogLogTest, EmptyEstimate) {
    EXPECT_EQ(hll->estimate(), 0u);
}

TEST_F(HyperLogLogTest, SingleElement) {
    hll->add(uint64_t(42));
    uint64_t estimate = hll->estimate();
    
    EXPECT_GE(estimate, 1u);
    EXPECT_LE(estimate, 10u);
}

TEST_F(HyperLogLogTest, DuplicatesDoNotIncrease) {
    for (int i = 0; i < 1000; ++i) {
        hll->add(uint64_t(42));
    }
    
    uint64_t estimate = hll->estimate();
    EXPECT_GE(estimate, 1u);
    EXPECT_LE(estimate, 10u);
}

TEST_F(HyperLogLogTest, Accuracy) {
    const uint64_t n = 100000;
    
    for (uint64_t i = 0; i < n; ++i) {
        hll->add(i);
    }
    
    uint64_t estimate = hll->estimate();
    double error = std::abs(static_cast<double>(estimate) - n) / n;
    
    EXPECT_LT(error, 0.05) << "Estimate: " << estimate << ", Actual: " << n;
}

TEST_F(HyperLogLogTest, Clear) {
    for (int i = 0; i < 1000; ++i) {
        hll->add(uint64_t(i));
    }
    
    EXPECT_GT(hll->estimate(), 0u);
    
    hll->clear();
    
    EXPECT_EQ(hll->estimate(), 0u);
}

TEST_F(HyperLogLogTest, Merge) {
    HyperLogLog hll1(10);
    HyperLogLog hll2(10);
    
    for (int i = 0; i < 5000; ++i) {
        hll1.add(uint64_t(i));
    }
    for (int i = 5000; i < 10000; ++i) {
        hll2.add(uint64_t(i));
    }
    
    hll1.merge(hll2);
    
    uint64_t estimate = hll1.estimate();
    double error = std::abs(static_cast<double>(estimate) - 10000) / 10000;
    
    EXPECT_LT(error, 0.1);
}

TEST_F(HyperLogLogTest, MergeDifferentPrecisionThrows) {
    HyperLogLog hll1(10);
    HyperLogLog hll2(12);
    
    EXPECT_THROW(hll1.merge(hll2), std::invalid_argument);
}

TEST_F(HyperLogLogTest, GetStandardError) {
    HyperLogLog hll14(14);
    double se = hll14.getStandardError();
    
    EXPECT_NEAR(se, 0.0081, 0.001);
}

TEST_F(HyperLogLogTest, GetMemoryUsage) {
    EXPECT_GT(hll->getMemoryUsage(), 0u);
}

TEST_F(HyperLogLogTest, GetName) {
    EXPECT_EQ(hll->getName(), "HyperLogLog");
}

TEST_F(HyperLogLogTest, InvalidPrecision) {
    EXPECT_THROW(HyperLogLog(3), std::invalid_argument);
    EXPECT_THROW(HyperLogLog(19), std::invalid_argument);
}

TEST_F(HyperLogLogTest, StringElements) {
    for (int i = 0; i < 10000; ++i) {
        hll->add(std::string("user_") + std::to_string(i));
    }
    
    uint64_t estimate = hll->estimate();
    double error = std::abs(static_cast<double>(estimate) - 10000) / 10000;
    
    EXPECT_LT(error, 0.05);
}

TEST_F(HyperLogLogTest, WithMurmurHasher) {
    auto hasher = std::make_shared<MurmurHasher>();
    HyperLogLog hll2(14, hasher);
    
    for (int i = 0; i < 10000; ++i) {
        hll2.add(uint64_t(i));
    }
    
    uint64_t estimate = hll2.estimate();
    double error = std::abs(static_cast<double>(estimate) - 10000) / 10000;
    
    EXPECT_LT(error, 0.05);
    EXPECT_EQ(hll2.getHasherName(), "MurmurHash3");
}

// ============================================
// Count-Min Sketch Tests
// ============================================

class CountMinSketchTest : public ::testing::Test {
protected:
    void SetUp() override {
        cms = std::make_unique<CountMinSketch>(0.01, 0.01);
    }
    
    std::unique_ptr<CountMinSketch> cms;
};

TEST_F(CountMinSketchTest, EmptyEstimate) {
    EXPECT_EQ(cms->estimate(uint64_t(42)), 0u);
}

TEST_F(CountMinSketchTest, SingleAdd) {
    cms->add(uint64_t(42));
    EXPECT_GE(cms->estimate(uint64_t(42)), 1u);
}

TEST_F(CountMinSketchTest, MultipleAdds) {
    for (int i = 0; i < 100; ++i) {
        cms->add(uint64_t(42));
    }
    
    uint64_t estimate = cms->estimate(uint64_t(42));
    EXPECT_GE(estimate, 100u);
}

TEST_F(CountMinSketchTest, AddWithCount) {
    cms->add(uint64_t(42), 50);
    
    uint64_t estimate = cms->estimate(uint64_t(42));
    EXPECT_GE(estimate, 50u);
}

TEST_F(CountMinSketchTest, NeverUnderestimates) {
    // CMS гарантирует: estimate(x) >= actual_count(x)
    // Это ключевая гарантия алгоритма!
    std::unordered_map<uint64_t, uint64_t> actual;
    std::mt19937_64 rng(42);
    std::uniform_int_distribution<uint64_t> dist(0, 999);
    
    for (int i = 0; i < 10000; ++i) {
        uint64_t elem = dist(rng);
        cms->add(elem);
        actual[elem]++;
    }
    
    for (const auto& [elem, count] : actual) {
        EXPECT_GE(cms->estimate(elem), count)
            << "Underestimate for element " << elem;
    }
}

TEST_F(CountMinSketchTest, Clear) {
    cms->add(uint64_t(42), 100);
    EXPECT_GE(cms->estimate(uint64_t(42)), 100u);
    
    cms->clear();
    
    EXPECT_EQ(cms->estimate(uint64_t(42)), 0u);
    EXPECT_EQ(cms->getTotalCount(), 0u);
}

TEST_F(CountMinSketchTest, GetTotalCount) {
    cms->add(uint64_t(1));
    cms->add(uint64_t(2));
    cms->add(uint64_t(1), 5);
    
    EXPECT_EQ(cms->getTotalCount(), 7u);
}

TEST_F(CountMinSketchTest, GetMemoryUsage) {
    EXPECT_GT(cms->getMemoryUsage(), 0u);
}

TEST_F(CountMinSketchTest, GetName) {
    EXPECT_EQ(cms->getName(), "CountMinSketch");
}

TEST_F(CountMinSketchTest, InvalidParameters) {
    EXPECT_THROW(CountMinSketch(0.0, 0.01), std::invalid_argument);
    EXPECT_THROW(CountMinSketch(1.0, 0.01), std::invalid_argument);
    EXPECT_THROW(CountMinSketch(0.01, 0.0), std::invalid_argument);
    EXPECT_THROW(CountMinSketch(0.01, 1.0), std::invalid_argument);
}

TEST_F(CountMinSketchTest, ExplicitConstructorWithHasher) {
    auto hasher = std::make_shared<StdHasher>();
    CountMinSketch cms2(size_t(1000), size_t(5), hasher);
    EXPECT_EQ(cms2.getWidth(), 1000u);
    EXPECT_EQ(cms2.getDepth(), 5u);
}

TEST_F(CountMinSketchTest, StringElements) {
    cms->add(std::string("video_A"));
    cms->add(std::string("video_A"));
    cms->add(std::string("video_B"));
    
    EXPECT_GE(cms->estimate(std::string("video_A")), 2u);
    EXPECT_GE(cms->estimate(std::string("video_B")), 1u);
}

TEST_F(CountMinSketchTest, WithMurmurHasher) {
    auto hasher = std::make_shared<MurmurHasher>();
    CountMinSketch cms2(0.01, 0.01, hasher);
    
    cms2.add(uint64_t(42), 100);
    EXPECT_GE(cms2.estimate(uint64_t(42)), 100u);
    EXPECT_EQ(cms2.getHasherName(), "MurmurHash3");
}

// ============================================
// Hasher Tests
// ============================================

TEST(HasherTest, StdHasherBasic) {
    StdHasher hasher;
    
    uint64_t h1 = hasher.hash(uint64_t(42), 0);
    uint64_t h2 = hasher.hash(uint64_t(42), 0);
    EXPECT_EQ(h1, h2);  // Детерминированность
    
    uint64_t h3 = hasher.hash(uint64_t(43), 0);
    EXPECT_NE(h1, h3);  // Разные входы — разные хэши
}

TEST(HasherTest, StdHasherWithSeed) {
    StdHasher hasher;
    
    uint64_t h1 = hasher.hash(uint64_t(42), 0);
    uint64_t h2 = hasher.hash(uint64_t(42), 1);
    EXPECT_NE(h1, h2);  // Разные seeds — разные хэши
}

TEST(HasherTest, MurmurHasherBasic) {
    MurmurHasher hasher;
    
    uint64_t h1 = hasher.hash(uint64_t(42), 0);
    uint64_t h2 = hasher.hash(uint64_t(42), 0);
    EXPECT_EQ(h1, h2);
    
    uint64_t h3 = hasher.hash(uint64_t(43), 0);
    EXPECT_NE(h1, h3);
}

TEST(HasherTest, MurmurHasherString) {
    MurmurHasher hasher;
    
    uint64_t h1 = hasher.hash(std::string("hello"), 0);
    uint64_t h2 = hasher.hash(std::string("hello"), 0);
    EXPECT_EQ(h1, h2);
    
    uint64_t h3 = hasher.hash(std::string("world"), 0);
    EXPECT_NE(h1, h3);
}

// ============================================
// Comparison Tests
// ============================================

TEST(ComparisonTest, BloomFilterVsSet) {
    BloomFilter bf(100000, 0.01);
    std::unordered_set<uint64_t> exactSet;
    
    for (uint64_t i = 0; i < 50000; ++i) {
        bf.add(i);
        exactSet.insert(i);
    }
    
    for (uint64_t i = 0; i < 50000; ++i) {
        EXPECT_TRUE(bf.contains(i));
        EXPECT_TRUE(exactSet.count(i) > 0);
    }
}

TEST(ComparisonTest, HyperLogLogVsSet) {
    HyperLogLog hll(14);
    std::unordered_set<uint64_t> exactSet;
    
    const uint64_t n = 100000;
    
    for (uint64_t i = 0; i < n; ++i) {
        hll.add(i);
        exactSet.insert(i);
    }
    
    uint64_t hllEstimate = hll.estimate();
    size_t exactCount = exactSet.size();
    
    double error = std::abs(static_cast<double>(hllEstimate) - exactCount) / exactCount;
    EXPECT_LT(error, 0.05);
}

TEST(ComparisonTest, CountMinSketchVsMap) {
    CountMinSketch cms(0.01, 0.01);
    std::unordered_map<uint64_t, uint64_t> exactMap;
    
    std::mt19937_64 rng(42);
    std::uniform_int_distribution<uint64_t> dist(0, 999);
    
    for (int i = 0; i < 100000; ++i) {
        uint64_t elem = dist(rng);
        cms.add(elem);
        exactMap[elem]++;
    }
    
    for (const auto& [elem, count] : exactMap) {
        EXPECT_GE(cms.estimate(elem), count);
    }
}

// ============================================
// Сравнение хэшеров
// ============================================

TEST(HasherComparisonTest, BloomFilterStdVsMurmur) {
    auto stdHasher = std::make_shared<StdHasher>();
    auto murmurHasher = std::make_shared<MurmurHasher>();
    
    BloomFilter bfStd(10000, 0.01, stdHasher);
    BloomFilter bfMurmur(10000, 0.01, murmurHasher);
    
    // Добавляем одинаковые элементы
    for (uint64_t i = 0; i < 5000; ++i) {
        bfStd.add(i);
        bfMurmur.add(i);
    }
    
    // Проверяем FPR
    int fpStd = 0, fpMurmur = 0;
    const int tests = 5000;
    for (uint64_t i = 5000; i < 5000 + tests; ++i) {
        if (bfStd.contains(i)) ++fpStd;
        if (bfMurmur.contains(i)) ++fpMurmur;
    }
    
    // Оба должны иметь низкий FPR
    EXPECT_LT(static_cast<double>(fpStd) / tests, 0.05);
    EXPECT_LT(static_cast<double>(fpMurmur) / tests, 0.05);
}
