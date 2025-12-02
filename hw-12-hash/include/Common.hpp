#pragma once

#include <vector>
#include <cmath>

/**
 * @brief Общие структуры и утилиты для хеш-таблиц
 */

/**
 * @brief Состояние ячейки в открытой адресации
 */
enum class CellState {
    EMPTY,
    ACTIVE,
    DELETED
};

/**
 * @brief Ячейка хеш-таблицы с открытой адресацией
 */
struct Cell {
    int key = -1;
    int value = -1;
    CellState state = CellState::EMPTY;
};

/**
 * @brief Узел цепочки для метода цепочек
 */
struct ListNode {
    int key;
    int value;
    ListNode* next = nullptr;

    ListNode(int k, int v) : key(k), value(v) {}
    // Деструктор НЕ удаляет next - это предотвращает double delete
    ~ListNode() = default;
};

/**
 * @brief Статистика хеш-таблицы
 */
struct HashTableStats {
    size_t total_elements = 0;
    size_t table_capacity = 0;
    size_t deleted_elements = 0;
    size_t collisions = 0;
    double load_factor = 0.0;
};

/**
 * @brief Конфигурация хеш-таблиц
 */
struct HashTableConfig {
    static constexpr double LOAD_FACTOR_THRESHOLD = 0.75;
    static constexpr double OPEN_ADDR_LOAD_FACTOR = 0.7;
    static constexpr double DELETE_RATIO_THRESHOLD = 0.25;
    static constexpr int MAX_PROBES = 100;
    static constexpr int INITIAL_CAPACITY = 11;
};

/**
 * @brief Утилиты для работы с простыми числами
 */
class PrimeUtils {
public:
    static bool isPrime(int n);
    static int getNextPrime(int n);
    static int getNextPrimeGreater(int n);
    static int getNextSizeForRehash(int n);
};
