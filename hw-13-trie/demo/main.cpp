/**
 * @file main.cpp
 * @brief Демонстрация работы библиотеки Trie
 * @author Anton
 * @date December 2025
 */

#include "trie.hpp"
#include <iostream>
#include <iomanip>


void printSeparator(const std::string& title) {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "  " << title << "\n";
    std::cout << std::string(60, '=') << "\n\n";
}

/**
 * @brief Демонстрация базового Trie (Junior уровень)
 */
void demoBasicTrie() {
    printSeparator("BASIC TRIE (Junior Level)");
    
    Trie trie;
    
    // LeetCode Example
    std::cout << "LeetCode 208 Example:\n";
    std::cout << "-----------------------\n";
    
    trie.insert("apple");
    std::cout << "trie.insert(\"apple\")\n";
    
    std::cout << "trie.search(\"apple\")   -> " 
              << std::boolalpha << trie.search("apple") << "\n";    // true
    
    std::cout << "trie.search(\"app\")     -> " 
              << std::boolalpha << trie.search("app") << "\n";      // false
    
    std::cout << "trie.startsWith(\"app\") -> " 
              << std::boolalpha << trie.startsWith("app") << "\n";  // true
    
    trie.insert("app");
    std::cout << "trie.insert(\"app\")\n";
    
    std::cout << "trie.search(\"app\")     -> " 
              << std::boolalpha << trie.search("app") << "\n";      // true
    
    // Дополнительные примеры
    std::cout << "\n\nAdditional Examples:\n";
    std::cout << "--------------------\n";
    
    trie.insert("application");
    trie.insert("apply");
    trie.insert("banana");
    trie.insert("band");
    
    std::cout << "Inserted: application, apply, banana, band\n";
    std::cout << "Total words: " << trie.size() << "\n\n";
    
    // Autocomplete demo
    std::cout << "Words starting with 'app':\n";
    for (const auto& word : trie.getWordsWithPrefix("app")) {
        std::cout << "  - " << word << "\n";
    }
    
    std::cout << "\nWords starting with 'ban':\n";
    for (const auto& word : trie.getWordsWithPrefix("ban")) {
        std::cout << "  - " << word << "\n";
    }
    
    // Remove demo
    std::cout << "\nRemoving 'apple'...\n";
    trie.remove("apple");
    std::cout << "trie.search(\"apple\") -> " 
              << std::boolalpha << trie.search("apple") << "\n";
    std::cout << "trie.startsWith(\"app\") -> " 
              << std::boolalpha << trie.startsWith("app") << "\n";
    std::cout << "Total words: " << trie.size() << "\n";
}

/**
 * @brief Демонстрация TrieMap (Middle уровень)
 */
void demoTrieMap() {
    printSeparator("TRIE MAP - Associative Array (Middle Level)");
    
    TrieMap<int> phoneBook;
    
    std::cout << "Phone Book Example:\n";
    std::cout << "-------------------\n";
    
    phoneBook.put("alice", 1001);
    phoneBook.put("bob", 1002);
    phoneBook.put("charlie", 1003);
    phoneBook.put("alex", 1004);
    phoneBook.put("alexander", 1005);
    
    std::cout << "Added entries: alice, bob, charlie, alex, alexander\n\n";
    
    // Get
    std::cout << "phoneBook.get(\"alice\")     -> ";
    if (auto val = phoneBook.get("alice")) {
        std::cout << val.value() << "\n";
    }
    
    std::cout << "phoneBook.get(\"unknown\")   -> ";
    if (auto val = phoneBook.get("unknown")) {
        std::cout << val.value() << "\n";
    } else {
        std::cout << "not found\n";
    }
    
    // Prefix search
    std::cout << "\nEntries starting with 'al':\n";
    for (const auto& [key, value] : phoneBook.getByPrefix("al")) {
        std::cout << "  " << key << " -> " << value << "\n";
    }
    
    // Operator[]
    std::cout << "\nUsing operator[]:\n";
    phoneBook["david"] = 1006;
    std::cout << "phoneBook[\"david\"] = 1006\n";
    std::cout << "phoneBook[\"david\"] -> " << phoneBook["david"] << "\n";
    
    // All keys
    std::cout << "\nAll keys in phone book:\n";
    for (const auto& key : phoneBook.keys()) {
        std::cout << "  - " << key << "\n";
    }
    
    std::cout << "\nTotal entries: " << phoneBook.size() << "\n";
    
    // String values demo
    printSeparator("TRIE MAP with String Values");
    
    TrieMap<std::string> dictionary;
    
    dictionary.put("apple", "A round fruit with red or green skin");
    dictionary.put("application", "A formal request or a software program");
    dictionary.put("apply", "To make a formal request");
    dictionary.put("banana", "A long curved yellow fruit");
    
    std::cout << "Dictionary entries starting with 'app':\n";
    for (const auto& [word, definition] : dictionary.getByPrefix("app")) {
        std::cout << "\n  " << word << ":\n";
        std::cout << "    " << definition << "\n";
    }
}

/**
 * @brief Демонстрация автодополнения
 */
void demoAutocomplete() {
    printSeparator("AUTOCOMPLETE DEMO");
    
    Trie trie;
    
    // Загружаем "словарь"
    std::vector<std::string> words = {
        "car", "card", "care", "careful", "careless",
        "cat", "catch", "category",
        "computer", "compute", "computing",
        "code", "coder", "coding",
        "coffee", "cold", "collection"
    };
    
    for (const auto& word : words) {
        trie.insert(word);
    }
    
    std::cout << "Loaded " << trie.size() << " words into dictionary.\n\n";
    
    // Симуляция ввода пользователя
    std::vector<std::string> prefixes = {"c", "ca", "car", "cod", "comp"};
    
    for (const auto& prefix : prefixes) {
        std::cout << "User types: \"" << prefix << "\"\n";
        std::cout << "Suggestions: ";
        
        auto suggestions = trie.getWordsWithPrefix(prefix);
        
        // Ограничим до 5 подсказок
        size_t limit = std::min(suggestions.size(), size_t(5));
        for (size_t i = 0; i < limit; ++i) {
            std::cout << suggestions[i];
            if (i < limit - 1) std::cout << ", ";
        }
        if (suggestions.size() > 5) {
            std::cout << " ... (" << suggestions.size() - 5 << " more)";
        }
        std::cout << "\n\n";
    }
}

int main() {
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════════╗\n";
    std::cout << "║          TRIE (PREFIX TREE) LIBRARY DEMO                 ║\n";
    std::cout << "║                   HW-13 Solution                         ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════╝\n";
    
    demoBasicTrie();
    demoTrieMap();
    demoAutocomplete();
    
    printSeparator("END OF DEMO");
    
    return 0;
}