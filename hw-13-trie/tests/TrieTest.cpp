/**
 * @file TrieTest.cpp
 * @brief Unit-тесты для класса Trie
 * @author Anton
 * @date December 2025
 */

#include <gtest/gtest.h>
#include "trie/Trie.hpp"
#include <algorithm>


class TrieTest : public ::testing::Test {
protected:
    Trie trie;
};

// ============= Insert Tests =============

TEST_F(TrieTest, InsertSingleWord) {
    trie.insert("apple");
    EXPECT_EQ(trie.size(), 1);
    EXPECT_TRUE(trie.search("apple"));
}

TEST_F(TrieTest, InsertMultipleWords) {
    trie.insert("apple");
    trie.insert("app");
    trie.insert("application");
    EXPECT_EQ(trie.size(), 3);
}

TEST_F(TrieTest, InsertDuplicateWord) {
    trie.insert("test");
    trie.insert("test");
    EXPECT_EQ(trie.size(), 1);
}

TEST_F(TrieTest, InsertEmptyString) {
    trie.insert("");
    EXPECT_EQ(trie.size(), 1);
    EXPECT_TRUE(trie.search(""));
}

// ============= Search Tests =============

TEST_F(TrieTest, SearchExistingWord) {
    trie.insert("hello");
    EXPECT_TRUE(trie.search("hello"));
}

TEST_F(TrieTest, SearchNonExistingWord) {
    trie.insert("hello");
    EXPECT_FALSE(trie.search("world"));
}

TEST_F(TrieTest, SearchPrefixNotWord) {
    trie.insert("apple");
    EXPECT_FALSE(trie.search("app"));
}

TEST_F(TrieTest, SearchInEmptyTrie) {
    EXPECT_FALSE(trie.search("anything"));
}

TEST_F(TrieTest, SearchEmptyString) {
    EXPECT_FALSE(trie.search(""));
    trie.insert("");
    EXPECT_TRUE(trie.search(""));
}

// ============= StartsWith Tests =============

TEST_F(TrieTest, StartsWithExistingPrefix) {
    trie.insert("apple");
    EXPECT_TRUE(trie.startsWith("app"));
    EXPECT_TRUE(trie.startsWith("appl"));
    EXPECT_TRUE(trie.startsWith("apple"));
}

TEST_F(TrieTest, StartsWithNonExistingPrefix) {
    trie.insert("apple");
    EXPECT_FALSE(trie.startsWith("ban"));
    EXPECT_FALSE(trie.startsWith("application"));
}

TEST_F(TrieTest, StartsWithEmptyPrefix) {
    trie.insert("apple");
    EXPECT_TRUE(trie.startsWith(""));
}

TEST_F(TrieTest, StartsWithSingleChar) {
    trie.insert("apple");
    EXPECT_TRUE(trie.startsWith("a"));
    EXPECT_FALSE(trie.startsWith("b"));
}

// ============= Remove Tests =============

TEST_F(TrieTest, RemoveExistingWord) {
    trie.insert("apple");
    EXPECT_TRUE(trie.remove("apple"));
    EXPECT_FALSE(trie.search("apple"));
    EXPECT_EQ(trie.size(), 0);
}

TEST_F(TrieTest, RemoveNonExistingWord) {
    trie.insert("apple");
    EXPECT_FALSE(trie.remove("app"));
    EXPECT_EQ(trie.size(), 1);
}

TEST_F(TrieTest, RemovePreservesOtherWords) {
    trie.insert("apple");
    trie.insert("app");
    trie.remove("apple");
    EXPECT_FALSE(trie.search("apple"));
    EXPECT_TRUE(trie.search("app"));
    EXPECT_EQ(trie.size(), 1);
}

TEST_F(TrieTest, RemovePrefixPreservesLongerWord) {
    trie.insert("apple");
    trie.insert("app");
    trie.remove("app");
    EXPECT_FALSE(trie.search("app"));
    EXPECT_TRUE(trie.search("apple"));
    EXPECT_TRUE(trie.startsWith("app"));
}

TEST_F(TrieTest, RemoveFromEmptyTrie) {
    EXPECT_FALSE(trie.remove("anything"));
}

// ============= GetWordsWithPrefix Tests =============

TEST_F(TrieTest, GetWordsWithPrefixBasic) {
    trie.insert("apple");
    trie.insert("app");
    trie.insert("application");
    trie.insert("banana");
    
    auto words = trie.getWordsWithPrefix("app");
    EXPECT_EQ(words.size(), 3);
    
    std::sort(words.begin(), words.end());
    EXPECT_EQ(words[0], "app");
    EXPECT_EQ(words[1], "apple");
    EXPECT_EQ(words[2], "application");
}

TEST_F(TrieTest, GetWordsWithPrefixNoMatch) {
    trie.insert("apple");
    auto words = trie.getWordsWithPrefix("xyz");
    EXPECT_TRUE(words.empty());
}

TEST_F(TrieTest, GetWordsWithEmptyPrefix) {
    trie.insert("a");
    trie.insert("b");
    trie.insert("c");
    
    auto words = trie.getWordsWithPrefix("");
    EXPECT_EQ(words.size(), 3);
}

TEST_F(TrieTest, GetWordsWithPrefixExactMatch) {
    trie.insert("test");
    auto words = trie.getWordsWithPrefix("test");
    EXPECT_EQ(words.size(), 1);
    EXPECT_EQ(words[0], "test");
}

// ============= Size and Empty Tests =============

TEST_F(TrieTest, EmptyTrieSize) {
    EXPECT_EQ(trie.size(), 0);
    EXPECT_TRUE(trie.empty());
}

TEST_F(TrieTest, SizeAfterInserts) {
    trie.insert("one");
    trie.insert("two");
    trie.insert("three");
    EXPECT_EQ(trie.size(), 3);
    EXPECT_FALSE(trie.empty());
}

TEST_F(TrieTest, SizeAfterRemoves) {
    trie.insert("one");
    trie.insert("two");
    trie.remove("one");
    EXPECT_EQ(trie.size(), 1);
}

// ============= Clear Tests =============

TEST_F(TrieTest, ClearEmptyTrie) {
    trie.clear();
    EXPECT_TRUE(trie.empty());
}

TEST_F(TrieTest, ClearNonEmptyTrie) {
    trie.insert("apple");
    trie.insert("banana");
    trie.clear();
    EXPECT_TRUE(trie.empty());
    EXPECT_FALSE(trie.search("apple"));
    EXPECT_FALSE(trie.search("banana"));
}

// ============= LeetCode Example Test =============

TEST_F(TrieTest, LeetCodeExample) {
    // Пример из задания LeetCode 208
    trie.insert("apple");
    EXPECT_TRUE(trie.search("apple"));   // true
    EXPECT_FALSE(trie.search("app"));    // false
    EXPECT_TRUE(trie.startsWith("app")); // true
    trie.insert("app");
    EXPECT_TRUE(trie.search("app"));     // true
}

// ============= Edge Cases =============

TEST_F(TrieTest, SingleCharacterWords) {
    for (char c = 'a'; c <= 'z'; ++c) {
        trie.insert(std::string(1, c));
    }
    EXPECT_EQ(trie.size(), 26);
    
    for (char c = 'a'; c <= 'z'; ++c) {
        EXPECT_TRUE(trie.search(std::string(1, c)));
    }
}

TEST_F(TrieTest, LongWord) {
    std::string longWord(2000, 'a');  // максимальная длина по условию
    trie.insert(longWord);
    EXPECT_TRUE(trie.search(longWord));
    EXPECT_TRUE(trie.startsWith(std::string(1000, 'a')));
}

TEST_F(TrieTest, ManyWordsWithCommonPrefix) {
    for (int i = 0; i < 100; ++i) {
        trie.insert("prefix" + std::string(i + 1, 'x'));
    }
    EXPECT_EQ(trie.size(), 100);
    EXPECT_EQ(trie.getWordsWithPrefix("prefix").size(), 100);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}