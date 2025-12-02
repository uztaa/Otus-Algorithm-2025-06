/**
 * @file Trie.cpp
 * @brief Реализация методов класса Trie
 * @author Anton
 * @date December 2025
 */

#include "trie/Trie.hpp"


Trie::Trie() 
    : root_(std::make_unique<TrieNode>())
    , wordCount_(0) {
}

void Trie::insert(const std::string& word) {
    TrieNode* current = root_.get();
    
    for (char c : word) {
        size_t index = TrieNode::charToIndex(c);
        if (!current->children[index]) {
            current->children[index] = std::make_unique<TrieNode>();
        }
        current = current->children[index].get();
    }
    
    if (!current->isEndOfWord) {
        current->isEndOfWord = true;
        ++wordCount_;
    }
}

bool Trie::search(const std::string& word) const {
    TrieNode* node = findNode(word);
    return node != nullptr && node->isEndOfWord;
}

bool Trie::startsWith(const std::string& prefix) const {
    return findNode(prefix) != nullptr;
}

bool Trie::remove(const std::string& word) {
    if (!search(word)) {
        return false;
    }
    
    removeHelper(root_.get(), word, 0);
    --wordCount_;
    return true;
}

std::vector<std::string> Trie::getWordsWithPrefix(const std::string& prefix) const {
    std::vector<std::string> results;
    TrieNode* node = findNode(prefix);
    
    if (node) {
        std::string current = prefix;
        collectWords(node, current, results);
    }
    
    return results;
}

size_t Trie::size() const {
    return wordCount_;
}

bool Trie::empty() const {
    return wordCount_ == 0;
}

void Trie::clear() {
    root_ = std::make_unique<TrieNode>();
    wordCount_ = 0;
}

TrieNode* Trie::findNode(const std::string& str) const {
    TrieNode* current = root_.get();
    
    for (char c : str) {
        size_t index = TrieNode::charToIndex(c);
        if (!current->children[index]) {
            return nullptr;
        }
        current = current->children[index].get();
    }
    
    return current;
}

void Trie::collectWords(TrieNode* node, std::string& current, 
                        std::vector<std::string>& results) const {
    if (node->isEndOfWord) {
        results.push_back(current);
    }
    
    for (size_t i = 0; i < ALPHABET_SIZE; ++i) {
        if (node->children[i]) {
            current.push_back(TrieNode::indexToChar(i));
            collectWords(node->children[i].get(), current, results);
            current.pop_back();
        }
    }
}

bool Trie::removeHelper(TrieNode* node, const std::string& word, size_t depth) {
    if (depth == word.size()) {
        node->isEndOfWord = false;
        return node->hasNoChildren();
    }
    
    size_t index = TrieNode::charToIndex(word[depth]);
    TrieNode* child = node->children[index].get();
    
    if (removeHelper(child, word, depth + 1)) {
        node->children[index].reset();
        return !node->isEndOfWord && node->hasNoChildren();
    }
    
    return false;
}
