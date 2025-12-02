#pragma once

/**
 * @file TrieMap.hpp
 * @brief Ассоциативный массив на основе Trie
 * @author Anton
 * @date December 2025
 * 
 * Реализация ассоциативного массива (key-value store) на базе префиксного дерева.
 * Уровень: Middle
 */


#include "ITrieMap.hpp"
#include "TrieNode.hpp"
#include <memory>


/**
 * @brief Ассоциативный массив на основе Trie
 * @tparam V Тип значения
 * 
 * Поддерживает операции:
 * - put: O(m)
 * - get: O(m)
 * - remove: O(m)
 * - contains: O(m)
 * - getByPrefix: O(m + k), где k — кол-во результатов
 * 
 * m — длина ключа
 */
template<typename V>
class TrieMap : public ITrieMap<V> {
public:
    /**
     * @brief Конструктор по умолчанию
     */
    TrieMap() : root_(std::make_unique<TrieMapNode<V>>()), entryCount_(0) {}
    
    /**
     * @brief Деструктор
     */
    ~TrieMap() override = default;
    
    // Запрет копирования
    TrieMap(const TrieMap&) = delete;
    TrieMap& operator=(const TrieMap&) = delete;
    
    // Разрешение перемещения
    TrieMap(TrieMap&&) noexcept = default;
    TrieMap& operator=(TrieMap&&) noexcept = default;
    
    // ITrieMap interface
    void put(const std::string& key, const V& value) override {
        TrieMapNode<V>* current = root_.get();
        
        for (char c : key) {
            size_t index = TrieMapNode<V>::charToIndex(c);
            if (!current->children[index]) {
                current->children[index] = std::make_unique<TrieMapNode<V>>();
            }
            current = current->children[index].get();
        }
        
        if (!current->hasValue()) {
            ++entryCount_;
        }
        current->value = value;
    }
    
    std::optional<V> get(const std::string& key) const override {
        TrieMapNode<V>* node = findNode(key);
        if (node && node->hasValue()) {
            return node->value;
        }
        return std::nullopt;
    }
    
    bool remove(const std::string& key) override {
        if (!contains(key)) {
            return false;
        }
        
        removeHelper(root_.get(), key, 0);
        --entryCount_;
        return true;
    }
    
    bool contains(const std::string& key) const override {
        TrieMapNode<V>* node = findNode(key);
        return node != nullptr && node->hasValue();
    }
    
    std::vector<std::pair<std::string, V>> getByPrefix(const std::string& prefix) const override {
        std::vector<std::pair<std::string, V>> results;
        TrieMapNode<V>* node = findNode(prefix);
        
        if (node) {
            std::string current = prefix;
            collectEntries(node, current, results);
        }
        
        return results;
    }
    
    std::vector<std::string> keysWithPrefix(const std::string& prefix) const override {
        std::vector<std::string> results;
        TrieMapNode<V>* node = findNode(prefix);
        
        if (node) {
            std::string current = prefix;
            collectKeys(node, current, results);
        }
        
        return results;
    }
    
    size_t size() const override {
        return entryCount_;
    }
    
    bool empty() const override {
        return entryCount_ == 0;
    }
    
    void clear() override {
        root_ = std::make_unique<TrieMapNode<V>>();
        entryCount_ = 0;
    }
    
    std::vector<std::string> keys() const override {
        return keysWithPrefix("");
    }
    
    /**
     * @brief Оператор доступа по ключу
     * @param key Ключ
     * @return Ссылка на значение (создаёт default если не существует)
     */
    V& operator[](const std::string& key) {
        TrieMapNode<V>* current = root_.get();
        
        for (char c : key) {
            size_t index = TrieMapNode<V>::charToIndex(c);
            if (!current->children[index]) {
                current->children[index] = std::make_unique<TrieMapNode<V>>();
            }
            current = current->children[index].get();
        }
        
        if (!current->hasValue()) {
            current->value = V{};
            ++entryCount_;
        }
        
        return current->value.value();
    }
    
private:
    std::unique_ptr<TrieMapNode<V>> root_;
    size_t entryCount_;
    
    /**
     * @brief Найти узел по ключу
     * @param key Ключ для поиска
     * @return Указатель на узел или nullptr
     */
    TrieMapNode<V>* findNode(const std::string& key) const {
        TrieMapNode<V>* current = root_.get();
        
        for (char c : key) {
            size_t index = TrieMapNode<V>::charToIndex(c);
            if (!current->children[index]) {
                return nullptr;
            }
            current = current->children[index].get();
        }
        
        return current;
    }
    
    /**
     * @brief Рекурсивный сбор пар ключ-значение
     */
    void collectEntries(TrieMapNode<V>* node, std::string& current, 
                        std::vector<std::pair<std::string, V>>& results) const {
        if (node->hasValue()) {
            results.emplace_back(current, node->value.value());
        }
        
        for (size_t i = 0; i < ALPHABET_SIZE; ++i) {
            if (node->children[i]) {
                current.push_back(TrieMapNode<V>::indexToChar(i));
                collectEntries(node->children[i].get(), current, results);
                current.pop_back();
            }
        }
    }
    
    /**
     * @brief Рекурсивный сбор ключей
     */
    void collectKeys(TrieMapNode<V>* node, std::string& current, 
                     std::vector<std::string>& results) const {
        if (node->hasValue()) {
            results.push_back(current);
        }
        
        for (size_t i = 0; i < ALPHABET_SIZE; ++i) {
            if (node->children[i]) {
                current.push_back(TrieMapNode<V>::indexToChar(i));
                collectKeys(node->children[i].get(), current, results);
                current.pop_back();
            }
        }
    }
    
    /**
     * @brief Рекурсивное удаление ключа
     */
    bool removeHelper(TrieMapNode<V>* node, const std::string& key, size_t depth) {
        if (depth == key.size()) {
            node->value = std::nullopt;
            return node->hasNoChildren();
        }
        
        size_t index = TrieMapNode<V>::charToIndex(key[depth]);
        TrieMapNode<V>* child = node->children[index].get();
        
        if (removeHelper(child, key, depth + 1)) {
            node->children[index].reset();
            return !node->hasValue() && node->hasNoChildren();
        }
        
        return false;
    }
};
