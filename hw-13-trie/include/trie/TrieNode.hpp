#pragma once

/**
 * @file TrieNode.hpp
 * @brief Узел префиксного дерева
 * @author Anton
 * @date December 2025
 */


#include <array>
#include <memory>
#include <optional>


/**
 * @brief Размер алфавита (lowercase English letters)
 */
constexpr size_t ALPHABET_SIZE = 26;

/**
 * @brief Узел базового Trie
 * 
 * Содержит массив указателей на потомков и флаг конца слова.
 * Используется для Junior уровня.
 */
struct TrieNode {
    std::array<std::unique_ptr<TrieNode>, ALPHABET_SIZE> children;
    bool isEndOfWord;
    
    TrieNode() : children{}, isEndOfWord(false) {}
    
    /**
     * @brief Проверить, есть ли у узла потомки
     * @return true если все потомки пусты
     */
    bool hasNoChildren() const {
        for (const auto& child : children) {
            if (child) return false;
        }
        return true;
    }
    
    /**
     * @brief Преобразовать символ в индекс массива
     * @param c Символ (a-z)
     * @return Индекс от 0 до 25
     */
    static size_t charToIndex(char c) {
        return static_cast<size_t>(c - 'a');
    }
    
    /**
     * @brief Преобразовать индекс в символ
     * @param index Индекс от 0 до 25
     * @return Символ (a-z)
     */
    static char indexToChar(size_t index) {
        return static_cast<char>('a' + index);
    }
};

/**
 * @brief Узел Trie для ассоциативного массива
 * @tparam V Тип хранимого значения
 * 
 * Расширение TrieNode для хранения значений.
 * Используется для Middle уровня.
 */
template<typename V>
struct TrieMapNode {
    std::array<std::unique_ptr<TrieMapNode<V>>, ALPHABET_SIZE> children;
    std::optional<V> value;
    
    TrieMapNode() : children{}, value(std::nullopt) {}
    
    /**
     * @brief Проверить, содержит ли узел значение
     * @return true если узел является концом ключа
     */
    bool hasValue() const {
        return value.has_value();
    }
    
    /**
     * @brief Проверить, есть ли у узла потомки
     * @return true если все потомки пусты
     */
    bool hasNoChildren() const {
        for (const auto& child : children) {
            if (child) return false;
        }
        return true;
    }
    
    /**
     * @brief Преобразовать символ в индекс массива
     * @param c Символ (a-z)
     * @return Индекс от 0 до 25
     */
    static size_t charToIndex(char c) {
        return static_cast<size_t>(c - 'a');
    }
    
    /**
     * @brief Преобразовать индекс в символ
     * @param index Индекс от 0 до 25
     * @return Символ (a-z)
     */
    static char indexToChar(size_t index) {
        return static_cast<char>('a' + index);
    }
};
