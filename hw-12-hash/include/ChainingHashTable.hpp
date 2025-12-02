#pragma once

#include "IHashTable.hpp"
#include "Common.hpp"
#include "HashFunction.hpp"
#include <vector>
#include <memory>

/**
 * @brief Хеш-таблица с цепочками для разрешения коллизий
 */
template<typename HashFunc = SimpleHashFunction<>>
class ChainingHashTable : public IHashTable {
private:
    std::vector<ListNode*> table_;
    size_t size_ = 0;
    int capacity_ = 0;
    HashFunc hash_func_;

    static constexpr double LOAD_FACTOR_THRESHOLD = 0.75;

    int hash(int key) const;
    void rehash();
    void deleteChain(ListNode* node);

public:
    explicit ChainingHashTable(int initial_capacity = 11);
    ~ChainingHashTable() override;

    void insert(int key, int value) override;
    bool search(int key, int& value) const override;
    bool remove(int key) override;

    size_t size() const override { return size_; }
    bool empty() const override { return size_ == 0; }
    double loadFactor() const override;
    void clear() override;
    std::string getName() const override;

    int getCapacity() const { return capacity_; }
    HashTableStats getStatistics() const;
};

// Template implementation
template<typename HashFunc>
ChainingHashTable<HashFunc>::ChainingHashTable(int initial_capacity)
    : capacity_(PrimeUtils::getNextPrime(initial_capacity)) {
    table_.assign(capacity_, nullptr);
}

template<typename HashFunc>
ChainingHashTable<HashFunc>::~ChainingHashTable() {
    clear();
}

template<typename HashFunc>
int ChainingHashTable<HashFunc>::hash(int key) const {
    int h = hash_func_(key, capacity_);
    return h < 0 ? -h : h;
}

template<typename HashFunc>
void ChainingHashTable<HashFunc>::deleteChain(ListNode* node) {
    while (node) {
        ListNode* temp = node;
        node = node->next;
        temp->next = nullptr;
        delete temp;
    }
}

template<typename HashFunc>
void ChainingHashTable<HashFunc>::rehash() {
    int old_capacity = capacity_;
    std::vector<ListNode*> old_table = table_;  // КОПИРУЕМ вектор!

    capacity_ = PrimeUtils::getNextPrimeGreater(capacity_);
    table_.assign(capacity_, nullptr);
    size_ = 0;

    // Вставляем элементы из старой таблицы
    for (int i = 0; i < old_capacity; ++i) {
        ListNode* current = old_table[i];
        while (current) {
            insert(current->key, current->value);
            current = current->next;
        }
    }

    // Очищаем старую таблицу (копию)
    for (int i = 0; i < old_capacity; ++i) {
        deleteChain(old_table[i]);
    }
}

template<typename HashFunc>
void ChainingHashTable<HashFunc>::insert(int key, int value) {
    if (loadFactor() > LOAD_FACTOR_THRESHOLD) {
        rehash();
    }

    int index = hash(key);
    ListNode* current = table_[index];

    while (current) {
        if (current->key == key) {
            current->value = value;
            return;
        }
        current = current->next;
    }

    ListNode* new_node = new ListNode(key, value);
    new_node->next = table_[index];
    table_[index] = new_node;
    size_++;
}

template<typename HashFunc>
bool ChainingHashTable<HashFunc>::search(int key, int& value) const {
    int index = hash(key);
    ListNode* current = table_[index];

    while (current) {
        if (current->key == key) {
            value = current->value;
            return true;
        }
        current = current->next;
    }
    return false;
}

template<typename HashFunc>
bool ChainingHashTable<HashFunc>::remove(int key) {
    int index = hash(key);
    ListNode* current = table_[index];
    ListNode* prev = nullptr;

    while (current) {
        if (current->key == key) {
            if (prev) {
                prev->next = current->next;
            } else {
                table_[index] = current->next;
            }
            current->next = nullptr;
            delete current;
            size_--;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}

template<typename HashFunc>
double ChainingHashTable<HashFunc>::loadFactor() const {
    return static_cast<double>(size_) / capacity_;
}

template<typename HashFunc>
void ChainingHashTable<HashFunc>::clear() {
    for (int i = 0; i < capacity_; ++i) {
        deleteChain(table_[i]);
        table_[i] = nullptr;
    }
    size_ = 0;
}

template<typename HashFunc>
std::string ChainingHashTable<HashFunc>::getName() const {
    return "ChainingHashTable";
}

template<typename HashFunc>
HashTableStats ChainingHashTable<HashFunc>::getStatistics() const {
    HashTableStats stats;
    stats.total_elements = size_;
    stats.table_capacity = capacity_;
    stats.load_factor = loadFactor();

    for (int i = 0; i < capacity_; ++i) {
        ListNode* current = table_[i];
        int chain_length = 0;
        while (current) {
            chain_length++;
            current = current->next;
        }
        if (chain_length > 1) {
            stats.collisions += chain_length - 1;
        }
    }

    return stats;
}
