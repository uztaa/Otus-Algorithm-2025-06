#include <gtest/gtest.h>

#include <memory>
#include "Record.h"
#include "../src/search/IndexSearcher.cpp"

/// Хелпер для создания вектора записей
std::vector<Record> makeRecords(std::vector<int> keys) {
    std::vector<Record> result;
    for (int k : keys) {
        result.emplace_back(k, "Value" + std::to_string(k));
    }
    return result;
}

class IndexSearcherTest : public ::testing::Test {
protected:
    IndexSearcher searcher;
};

TEST_F(IndexSearcherTest, HandlesEmptyArray) {
    std::vector<Record> data;
    EXPECT_EQ(searcher.search(data, 42), nullptr);
}

TEST_F(IndexSearcherTest, SingleElementFound) {
    auto data = makeRecords({ 42 });
    Record* res = searcher.search(data, 42);
    ASSERT_NE(res, nullptr);
    EXPECT_EQ(res->getKey(), 42);
}

TEST_F(IndexSearcherTest, SingleElementNotFound) {
    auto data = makeRecords({ 42 });
    EXPECT_EQ(searcher.search(data, 99), nullptr);
}

TEST_F(IndexSearcherTest, KeyAtBeginning) {
    auto data = makeRecords({ 10, 20, 30, 40, 50 });
    Record* res = searcher.search(data, 10);
    ASSERT_NE(res, nullptr);
    EXPECT_EQ(res->getKey(), 10);
}

TEST_F(IndexSearcherTest, KeyInMiddle) {
    auto data = makeRecords({ 10, 20, 30, 40, 50 });
    Record* res = searcher.search(data, 30);
    ASSERT_NE(res, nullptr);
    EXPECT_EQ(res->getKey(), 30);
}

TEST_F(IndexSearcherTest, KeyAtEnd) {
    auto data = makeRecords({ 10, 20, 30, 40, 50 });
    Record* res = searcher.search(data, 50);
    ASSERT_NE(res, nullptr);
    EXPECT_EQ(res->getKey(), 50);
}

TEST_F(IndexSearcherTest, KeyBeforeBeginning) {
    auto data = makeRecords({ 10, 20, 30 });
    EXPECT_EQ(searcher.search(data, 5), nullptr);
}

TEST_F(IndexSearcherTest, KeyAfterEnd) {
    auto data = makeRecords({ 10, 20, 30 });
    EXPECT_EQ(searcher.search(data, 99), nullptr);
}

TEST_F(IndexSearcherTest, KeyNotInDataButBetweenElements) {
    auto data = makeRecords({ 10, 20, 30, 40 });
    EXPECT_EQ(searcher.search(data, 25), nullptr);
}

TEST_F(IndexSearcherTest, KeyExactlyMatchesIndexBoundary) {
    auto data = makeRecords({
        10, 11, 12, 13, 14, 15, 16, 17,
        18, 19, 20
        });

    Record* res = searcher.search(data, 10);
    ASSERT_NE(res, nullptr);
    EXPECT_EQ(res->getKey(), 10);

    res = searcher.search(data, 18);
    ASSERT_NE(res, nullptr);
    EXPECT_EQ(res->getKey(), 18);
}

TEST_F(IndexSearcherTest, LargeArrayKeyNearBlockBoundaries) {
    std::vector<int> keys(100);
    for (int i = 0; i < 100; ++i)
        keys[i] = i;

    auto data = makeRecords(keys);
    for (int testKey : {7, 8, 15, 23, 31, 99}) {
        Record* res = searcher.search(data, testKey);
        ASSERT_NE(res, nullptr);
        EXPECT_EQ(res->getKey(), testKey);
    }
}

TEST_F(IndexSearcherTest, MultipleSameKeysReturnsFirst) {
    auto data = makeRecords({ 10, 20, 30, 30, 30, 40 });
    Record* res = searcher.search(data, 30);
    ASSERT_NE(res, nullptr);
    EXPECT_EQ(res->getKey(), 30);
    EXPECT_EQ(res, &data[2]);
}
