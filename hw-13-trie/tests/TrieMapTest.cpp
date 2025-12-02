/**
 * @file TrieMapTest.cpp
 * @brief Unit-тесты для класса TrieMap
 * @author Anton
 * @date December 2025
 */

#include <gtest/gtest.h>
#include "trie/TrieMap.hpp"
#include <algorithm>


class TrieMapTest : public ::testing::Test {
protected:
    TrieMap<int> intMap;
    TrieMap<std::string> stringMap;
};

// ============= Put/Get Tests =============

TEST_F(TrieMapTest, PutAndGetSingle) {
    intMap.put("key", 42);
    auto result = intMap.get("key");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 42);
}

TEST_F(TrieMapTest, PutAndGetMultiple) {
    intMap.put("one", 1);
    intMap.put("two", 2);
    intMap.put("three", 3);
    
    EXPECT_EQ(intMap.get("one").value(), 1);
    EXPECT_EQ(intMap.get("two").value(), 2);
    EXPECT_EQ(intMap.get("three").value(), 3);
}

TEST_F(TrieMapTest, PutOverwrite) {
    intMap.put("key", 1);
    intMap.put("key", 2);
    
    EXPECT_EQ(intMap.get("key").value(), 2);
    EXPECT_EQ(intMap.size(), 1);
}

TEST_F(TrieMapTest, GetNonExisting) {
    intMap.put("exists", 1);
    auto result = intMap.get("notexists");
    EXPECT_FALSE(result.has_value());
}

TEST_F(TrieMapTest, PutEmptyKey) {
    intMap.put("", 100);
    EXPECT_EQ(intMap.get("").value(), 100);
}

// ============= Contains Tests =============

TEST_F(TrieMapTest, ContainsExisting) {
    intMap.put("test", 1);
    EXPECT_TRUE(intMap.contains("test"));
}

TEST_F(TrieMapTest, ContainsNonExisting) {
    intMap.put("test", 1);
    EXPECT_FALSE(intMap.contains("testing"));
}

TEST_F(TrieMapTest, ContainsPrefixButNotKey) {
    intMap.put("testing", 1);
    EXPECT_FALSE(intMap.contains("test"));
}

// ============= Remove Tests =============

TEST_F(TrieMapTest, RemoveExisting) {
    intMap.put("key", 42);
    EXPECT_TRUE(intMap.remove("key"));
    EXPECT_FALSE(intMap.contains("key"));
    EXPECT_EQ(intMap.size(), 0);
}

TEST_F(TrieMapTest, RemoveNonExisting) {
    intMap.put("key", 42);
    EXPECT_FALSE(intMap.remove("notkey"));
    EXPECT_EQ(intMap.size(), 1);
}

TEST_F(TrieMapTest, RemovePreservesOthers) {
    intMap.put("apple", 1);
    intMap.put("app", 2);
    intMap.remove("apple");
    
    EXPECT_FALSE(intMap.contains("apple"));
    EXPECT_TRUE(intMap.contains("app"));
    EXPECT_EQ(intMap.get("app").value(), 2);
}

TEST_F(TrieMapTest, RemovePrefixPreservesLonger) {
    intMap.put("apple", 1);
    intMap.put("app", 2);
    intMap.remove("app");
    
    EXPECT_FALSE(intMap.contains("app"));
    EXPECT_TRUE(intMap.contains("apple"));
}

// ============= GetByPrefix Tests =============

TEST_F(TrieMapTest, GetByPrefixBasic) {
    intMap.put("apple", 1);
    intMap.put("app", 2);
    intMap.put("application", 3);
    intMap.put("banana", 4);
    
    auto results = intMap.getByPrefix("app");
    EXPECT_EQ(results.size(), 3);
    
    // Проверим, что все нужные элементы присутствуют
    std::vector<std::string> keys;
    for (const auto& [key, value] : results) {
        keys.push_back(key);
    }
    std::sort(keys.begin(), keys.end());
    
    EXPECT_EQ(keys[0], "app");
    EXPECT_EQ(keys[1], "apple");
    EXPECT_EQ(keys[2], "application");
}

TEST_F(TrieMapTest, GetByPrefixNoMatch) {
    intMap.put("apple", 1);
    auto results = intMap.getByPrefix("xyz");
    EXPECT_TRUE(results.empty());
}

TEST_F(TrieMapTest, GetByPrefixEmptyPrefix) {
    intMap.put("a", 1);
    intMap.put("b", 2);
    auto results = intMap.getByPrefix("");
    EXPECT_EQ(results.size(), 2);
}

// ============= KeysWithPrefix Tests =============

TEST_F(TrieMapTest, KeysWithPrefixBasic) {
    intMap.put("car", 1);
    intMap.put("card", 2);
    intMap.put("care", 3);
    intMap.put("cat", 4);
    
    auto keys = intMap.keysWithPrefix("car");
    EXPECT_EQ(keys.size(), 3);
    
    std::sort(keys.begin(), keys.end());
    EXPECT_EQ(keys[0], "car");
    EXPECT_EQ(keys[1], "card");
    EXPECT_EQ(keys[2], "care");
}

// ============= Keys Tests =============

TEST_F(TrieMapTest, KeysEmpty) {
    auto keys = intMap.keys();
    EXPECT_TRUE(keys.empty());
}

TEST_F(TrieMapTest, KeysNonEmpty) {
    intMap.put("a", 1);
    intMap.put("b", 2);
    intMap.put("c", 3);
    
    auto keys = intMap.keys();
    EXPECT_EQ(keys.size(), 3);
    
    std::sort(keys.begin(), keys.end());
    EXPECT_EQ(keys[0], "a");
    EXPECT_EQ(keys[1], "b");
    EXPECT_EQ(keys[2], "c");
}

// ============= Size and Empty Tests =============

TEST_F(TrieMapTest, EmptyMapSize) {
    EXPECT_EQ(intMap.size(), 0);
    EXPECT_TRUE(intMap.empty());
}

TEST_F(TrieMapTest, SizeAfterOperations) {
    intMap.put("one", 1);
    intMap.put("two", 2);
    EXPECT_EQ(intMap.size(), 2);
    
    intMap.put("two", 22);  // overwrite
    EXPECT_EQ(intMap.size(), 2);
    
    intMap.remove("one");
    EXPECT_EQ(intMap.size(), 1);
}

// ============= Clear Tests =============

TEST_F(TrieMapTest, ClearMap) {
    intMap.put("a", 1);
    intMap.put("b", 2);
    intMap.clear();
    
    EXPECT_TRUE(intMap.empty());
    EXPECT_FALSE(intMap.contains("a"));
    EXPECT_FALSE(intMap.contains("b"));
}

// ============= Operator[] Tests =============

TEST_F(TrieMapTest, OperatorBracketRead) {
    intMap.put("key", 42);
    EXPECT_EQ(intMap["key"], 42);
}

TEST_F(TrieMapTest, OperatorBracketCreate) {
    intMap["newkey"] = 100;
    EXPECT_EQ(intMap.get("newkey").value(), 100);
    EXPECT_EQ(intMap.size(), 1);
}

TEST_F(TrieMapTest, OperatorBracketModify) {
    intMap["key"] = 1;
    intMap["key"] = 2;
    EXPECT_EQ(intMap["key"], 2);
    EXPECT_EQ(intMap.size(), 1);
}

// ============= String Value Tests =============

TEST_F(TrieMapTest, StringValues) {
    stringMap.put("greeting", "hello");
    stringMap.put("farewell", "goodbye");
    
    EXPECT_EQ(stringMap.get("greeting").value(), "hello");
    EXPECT_EQ(stringMap.get("farewell").value(), "goodbye");
}

// ============= Complex Key Patterns =============

TEST_F(TrieMapTest, OverlappingKeys) {
    intMap.put("a", 1);
    intMap.put("ab", 2);
    intMap.put("abc", 3);
    intMap.put("abcd", 4);
    intMap.put("abcde", 5);
    
    EXPECT_EQ(intMap.size(), 5);
    
    for (int i = 1; i <= 5; ++i) {
        std::string key(i, 'a');
        for (size_t j = 1; j < key.size(); ++j) {
            key[j] = 'a' + j;
        }
    }
    
    // Удаляем средний элемент
    intMap.remove("abc");
    EXPECT_FALSE(intMap.contains("abc"));
    EXPECT_TRUE(intMap.contains("ab"));
    EXPECT_TRUE(intMap.contains("abcd"));
}

TEST_F(TrieMapTest, DivergingPaths) {
    intMap.put("cat", 1);
    intMap.put("car", 2);
    intMap.put("card", 3);
    intMap.put("care", 4);
    
    EXPECT_EQ(intMap.keysWithPrefix("ca").size(), 4);
    EXPECT_EQ(intMap.keysWithPrefix("car").size(), 3);
    EXPECT_EQ(intMap.keysWithPrefix("cat").size(), 1);
}

// ============= Edge Cases =============

TEST_F(TrieMapTest, SingleCharKeys) {
    for (char c = 'a'; c <= 'z'; ++c) {
        intMap.put(std::string(1, c), c - 'a');
    }
    EXPECT_EQ(intMap.size(), 26);
    
    for (char c = 'a'; c <= 'z'; ++c) {
        EXPECT_EQ(intMap.get(std::string(1, c)).value(), c - 'a');
    }
}

TEST_F(TrieMapTest, LongKey) {
    std::string longKey(1000, 'x');
    intMap.put(longKey, 42);
    EXPECT_EQ(intMap.get(longKey).value(), 42);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}