#pragma once

#include "IHashTable.hpp"
#include "Common.hpp"
#include "HashFunction.hpp"
#include <vector>
#include <stdexcept>

/**
 * @brief Хеш-таблица с открытой адресацией и квадратичным пробированием
 */
template<typename HashFunc = SimpleHashFunction<>, 
         typename SecondHashFunc = SecondaryHashFunction<>>
class OpenAddressingHashTable : public IHashTable {
private:
    std::vector<Cell> table_;
    size_t size_ = 0;
    size_t deleted_ = 0;
    int capacity_ = 0;
    double load_factor_max_ = 0.0;
    HashFunc hash_func_;
    SecondHashFunc secondary_hash_;

    static constexpr int MAX_PROBES = 100;
    static constexpr double DELETE_THRESHOLD = 0.25;

    int hash(int key) const;
    int hash2(int key) const;
    int quadraticProbe(int index, int i) const;
    void rehash();

public:
    explicit OpenAddressingHashTable(int initial_capacity = 11, 
                                     double load_factor_max = 0.7);

    void insert(int key, int value) override;
    bool search(int key, int& value) const override;
    bool remove(int key) override;

    size_t size() const override { return size_; }
    bool empty() const override { return size_ == 0; }
    double loadFactor() const override;
    void clear() override;
    std::string getName() const override;

    size_t getDeletedCount() const { return deleted_; }
    int getCapacity() const { return capacity_; }
    HashTableStats getStatistics() const;
};

// Template implementation
template<typename HashFunc, typename SecondHashFunc>
OpenAddressingHashTable<HashFunc, SecondHashFunc>::OpenAddressingHashTable(
    int initial_capacity, double load_factor_max)
    : capacity_(PrimeUtils::getNextPrime(initial_capacity)),
      load_factor_max_(load_factor_max) {
    table_.assign(capacity_, Cell());
}

template<typename HashFunc, typename SecondHashFunc>
int OpenAddressingHashTable<HashFunc, SecondHashFunc>::hash(int key) const {
    int h = hash_func_(key, capacity_);
    return h < 0 ? -h : h;
}

template<typename HashFunc, typename SecondHashFunc>
int OpenAddressingHashTable<HashFunc, SecondHashFunc>::hash2(int key) const {
    int h = secondary_hash_(key, capacity_);
    return h < 0 ? -h : h;
}

template<typename HashFunc, typename SecondHashFunc>
int OpenAddressingHashTable<HashFunc, SecondHashFunc>::quadraticProbe(int index, int i) const {
    return (index + i + i * i) % capacity_;
}

template<typename HashFunc, typename SecondHashFunc>
void OpenAddressingHashTable<HashFunc, SecondHashFunc>::rehash() {
    int old_capacity = capacity_;
    std::vector<Cell> old_table = table_;

    capacity_ = PrimeUtils::getNextPrimeGreater(capacity_);
    table_.assign(capacity_, Cell());
    size_ = 0;
    deleted_ = 0;

    for (int i = 0; i < old_capacity; ++i) {
        if (old_table[i].state == CellState::ACTIVE) {
            insert(old_table[i].key, old_table[i].value);
        }
    }
}

template<typename HashFunc, typename SecondHashFunc>
void OpenAddressingHashTable<HashFunc, SecondHashFunc>::insert(int key, int value) {
    if (loadFactor() > load_factor_max_ || 
        (deleted_ > 0 && static_cast<double>(deleted_) / capacity_ > DELETE_THRESHOLD)) {
        rehash();
    }

    int index = hash(key);
    int first_deleted = -1;

    for (int i = 0; i < MAX_PROBES; ++i) {
        int pos = quadraticProbe(index, i);

        if (table_[pos].state == CellState::ACTIVE) {
            if (table_[pos].key == key) {
                table_[pos].value = value;
                return;
            }
        }
        else if (table_[pos].state == CellState::EMPTY) {
            if (first_deleted != -1) {
                table_[first_deleted].key = key;
                table_[first_deleted].value = value;
                table_[first_deleted].state = CellState::ACTIVE;
                deleted_--;
            } else {
                table_[pos].key = key;
                table_[pos].value = value;
                table_[pos].state = CellState::ACTIVE;
            }
            size_++;
            return;
        }
        else if (table_[pos].state == CellState::DELETED && first_deleted == -1) {
            first_deleted = pos;
        }
    }

    throw std::runtime_error("Hash table is full (max probes exceeded)");
}

template<typename HashFunc, typename SecondHashFunc>
bool OpenAddressingHashTable<HashFunc, SecondHashFunc>::search(int key, int& value) const {
    int index = hash(key);

    for (int i = 0; i < MAX_PROBES; ++i) {
        int pos = quadraticProbe(index, i);

        if (table_[pos].state == CellState::EMPTY) {
            return false;
        }

        if (table_[pos].state == CellState::ACTIVE && table_[pos].key == key) {
            value = table_[pos].value;
            return true;
        }
    }

    return false;
}

template<typename HashFunc, typename SecondHashFunc>
bool OpenAddressingHashTable<HashFunc, SecondHashFunc>::remove(int key) {
    int index = hash(key);

    for (int i = 0; i < MAX_PROBES; ++i) {
        int pos = quadraticProbe(index, i);

        if (table_[pos].state == CellState::EMPTY) {
            return false;
        }

        if (table_[pos].state == CellState::ACTIVE && table_[pos].key == key) {
            table_[pos].state = CellState::DELETED;
            size_--;
            deleted_++;

            if (deleted_ > capacity_ * DELETE_THRESHOLD) {
                const_cast<OpenAddressingHashTable*>(this)->rehash();
            }

            return true;
        }
    }

    return false;
}

template<typename HashFunc, typename SecondHashFunc>
double OpenAddressingHashTable<HashFunc, SecondHashFunc>::loadFactor() const {
    return static_cast<double>(size_) / capacity_;
}

template<typename HashFunc, typename SecondHashFunc>
void OpenAddressingHashTable<HashFunc, SecondHashFunc>::clear() {
    table_.assign(capacity_, Cell());
    size_ = 0;
    deleted_ = 0;
}

template<typename HashFunc, typename SecondHashFunc>
std::string OpenAddressingHashTable<HashFunc, SecondHashFunc>::getName() const {
    return "OpenAddressingHashTable (Quadratic Probing + Lazy Deletion)";
}

template<typename HashFunc, typename SecondHashFunc>
HashTableStats OpenAddressingHashTable<HashFunc, SecondHashFunc>::getStatistics() const {
    HashTableStats stats;
    stats.total_elements = size_;
    stats.table_capacity = capacity_;
    stats.deleted_elements = deleted_;
    stats.load_factor = loadFactor();
    return stats;
}
