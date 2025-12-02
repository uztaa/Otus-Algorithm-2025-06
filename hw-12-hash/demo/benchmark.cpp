#include "ChainingHashTable.hpp"
#include "OpenAddressingHashTable.hpp"
#include "HashFunction.hpp"
#include <iostream>
#include <iomanip>
#include <chrono>

// Макрос для измерения времени выполнения
#define MEASURE_TIME(name, code) { \
    auto start = std::chrono::high_resolution_clock::now(); \
    code; \
    auto end = std::chrono::high_resolution_clock::now(); \
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start); \
    std::cout << std::setw(35) << std::left << name \
              << std::setw(10) << duration.count() << " ms\n"; \
}

int main() {
    // Заголовок
    std::cout << "\n";
    std::cout << "====================================================================\n";
    std::cout << "                    HASH TABLES BENCHMARK                          \n";
    std::cout << "                 Chaining vs Open Addressing                        \n";
    std::cout << "====================================================================\n\n";

    const int N_INSERT = 100000;
    const int N_SEARCH = 10000;
    const int N_DELETE = 50000;

    // ========== ТЕСТЫ ВСТАВКИ ==========
    std::cout << ">>> TEST 1: INSERT " << N_INSERT << " ELEMENTS\n";
    std::cout << "--------------------------------------------------------------------\n";

    MEASURE_TIME("ChainingHashTable:", {
        ChainingHashTable<SimpleHashFunction<>> table;
        for (int i = 0; i < N_INSERT; ++i) {
            table.insert(i, i * 2);
        }
    });

    MEASURE_TIME("OpenAddressingHashTable:", {
        OpenAddressingHashTable<> table;
        for (int i = 0; i < N_INSERT; ++i) {
            table.insert(i, i * 2);
        }
    });

    std::cout << "\n";

    // ========== ТЕСТЫ ПОИСКА ==========
    std::cout << ">>> TEST 2: SEARCH " << N_SEARCH << " ELEMENTS\n";
    std::cout << "(in table with " << N_INSERT << " elements)\n";
    std::cout << "--------------------------------------------------------------------\n";

    MEASURE_TIME("ChainingHashTable:", {
        ChainingHashTable<SimpleHashFunction<>> table;
        // Вставляем элементы
        for (int i = 0; i < N_INSERT; ++i) {
            table.insert(i, i * 2);
        }
        // Ищем элементы
        for (int i = 0; i < N_SEARCH; ++i) {
            int val;
            table.search(i % N_INSERT, val);
        }
    });

    MEASURE_TIME("OpenAddressingHashTable:", {
        OpenAddressingHashTable<> table;
        // Вставляем элементы
        for (int i = 0; i < N_INSERT; ++i) {
            table.insert(i, i * 2);
        }
        // Ищем элементы
        for (int i = 0; i < N_SEARCH; ++i) {
            int val;
            table.search(i % N_INSERT, val);
        }
    });

    std::cout << "\n";

    // ========== ТЕСТЫ УДАЛЕНИЯ ==========
    std::cout << ">>> TEST 3: DELETE " << N_DELETE << " ELEMENTS\n";
    std::cout << "(from table with " << N_INSERT << " elements)\n";
    std::cout << "--------------------------------------------------------------------\n";

    MEASURE_TIME("ChainingHashTable:", {
        ChainingHashTable<SimpleHashFunction<>> table;
        // Вставляем элементы
        for (int i = 0; i < N_INSERT; ++i) {
            table.insert(i, i * 2);
        }
        // Удаляем элементы
        for (int i = 0; i < N_DELETE; ++i) {
            table.remove(i);
        }
    });

    MEASURE_TIME("OpenAddressingHashTable:", {
        OpenAddressingHashTable<> table;
        // Вставляем элементы
        for (int i = 0; i < N_INSERT; ++i) {
            table.insert(i, i * 2);
        }
        // Удаляем элементы (ленивое удаление)
        for (int i = 0; i < N_DELETE; ++i) {
            table.remove(i);
        }
    });

    std::cout << "\n";

    // ========== АНАЛИЗ ==========
    std::cout << "====================================================================\n";
    std::cout << "                         ANALYSIS                                   \n";
    std::cout << "====================================================================\n\n";

    std::cout << "Chaining:\n";
    std::cout << "  + Faster deletion (no rehashing overhead)\n";
    std::cout << "  + Can work with load factor > 1.0\n";
    std::cout << "  - Slower insert/search (poor cache locality)\n";
    std::cout << "  - Extra memory for pointers\n\n";

    std::cout << "Open Addressing:\n";
    std::cout << "  + Faster insert/search (better cache locality)\n";
    std::cout << "  + Less memory overhead\n";
    std::cout << "  + Better CPU cache utilization\n";
    std::cout << "  - Lazy deletion may cause fragmentation\n";
    std::cout << "  - Load factor must stay < 1.0\n\n";

    std::cout << "====================================================================\n";
    std::cout << "Conclusion: Open Addressing is better for read-heavy workloads\n";
    std::cout << "            Chaining is better for delete-heavy workloads\n";
    std::cout << "====================================================================\n\n";

    return 0;
}
