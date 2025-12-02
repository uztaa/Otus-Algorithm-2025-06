#include "OpenAddressingHashTable.hpp"
#include "HashFunction.hpp"
#include <iostream>
#include <iomanip>

void printHeader(const std::string& title) {
    std::cout << "\n";
    std::cout << std::string(70, '=') << "\n";
    std::cout << title << "\n";
    std::cout << std::string(70, '=') << "\n\n";
}

void printSection(const std::string& section) {
    std::cout << ">>> " << section << "\n";
    std::cout << std::string(70, '-') << "\n";
}

// Demo 1: Базовые операции
void demo1_BasicOperations() {
    printSection("1. BASIC OPERATIONS");
    
    // Создаем таблицу
    OpenAddressingHashTable<> table(11, 0.7);
    
    std::cout << "Inserting: 42->100, 10->200, 37->370, 8->80\n";
    table.insert(42, 100);
    table.insert(10, 200);
    table.insert(37, 370);
    table.insert(8, 80);
    
    std::cout << "Size: " << table.size() 
              << " | Deleted: " << table.getDeletedCount()
              << " | Load factor: " << std::fixed << std::setprecision(3) 
              << table.loadFactor() << "\n\n";
    
    std::cout << "Search for key 42: ";
    int value;
    if (table.search(42, value)) {
        std::cout << "FOUND, value = " << value << "\n";
    }
}

// Demo 2: Ленивое удаление
void demo2_LazyDeletion() {
    printSection("2. LAZY DELETION");
    
    // Создаем таблицу и вставляем элементы
    OpenAddressingHashTable<> table(11, 0.7);
    
    std::cout << "Inserting 10 elements...\n";
    for (int i = 1; i <= 10; ++i) {
        table.insert(i, i * 100);
    }
    
    std::cout << "Size: " << table.size() 
              << " | Deleted: " << table.getDeletedCount() << "\n\n";
    
    // Удаляем половину элементов
    std::cout << "Removing elements 2, 4, 6, 8, 10 (half)...\n";
    table.remove(2);
    table.remove(4);
    table.remove(6);
    table.remove(8);
    table.remove(10);
    
    std::cout << "Size: " << table.size() 
              << " | Deleted (DELETED cells): " << table.getDeletedCount() << "\n\n";
    
    // Проверяем оставшиеся элементы
    std::cout << "Searching remaining elements:\n";
    int val;
    std::cout << "  Key 1: ";
    if (table.search(1, val)) std::cout << "FOUND\n";
    else std::cout << "NOT FOUND\n";
    
    std::cout << "  Key 2 (deleted): ";
    if (table.search(2, val)) std::cout << "FOUND\n";
    else std::cout << "NOT FOUND (as expected)\n";
}

// Demo 3: Квадратичное пробирование
void demo3_QuadraticProbing() {
    printSection("3. QUADRATIC PROBING");
    
    std::cout << "Formula: h(k,i) = (h'(k) + i + i^2) mod m\n\n";
    std::cout << "For primary hash=5, capacity=11:\n";
    std::cout << "  i=0: (5 + 0 + 0)  mod 11 = 5\n";
    std::cout << "  i=1: (5 + 1 + 1)  mod 11 = 7\n";
    std::cout << "  i=2: (5 + 2 + 4)  mod 11 = 0\n";
    std::cout << "  i=3: (5 + 3 + 9)  mod 11 = 6\n";
    std::cout << "  i=4: (5 + 4 + 16) mod 11 = 3\n\n";
    
    std::cout << "This minimizes clustering:\n";
    std::cout << "  + No continuous chains like linear probing\n";
    std::cout << "  + Better distribution across table\n";
}

// Demo 4: Автоматический rehashing
void demo4_Rehashing() {
    printSection("4. AUTOMATIC REHASHING");
    
    // Маленькая таблица для демонстрации rehash
    OpenAddressingHashTable<> table(5, 0.7);
    
    std::cout << "Initial capacity: " << table.getCapacity() << "\n";
    std::cout << "Load factor threshold: 0.7\n\n";
    
    // Вставляем элементы
    for (int i = 1; i <= 15; ++i) {
        int old_capacity = table.getCapacity();
        table.insert(i, i * 10);
        
        if (table.getCapacity() != old_capacity) {
            std::cout << "REHASH: " << old_capacity << " -> " 
                      << table.getCapacity() << "\n";
        }
    }
    
    std::cout << "\nFinal capacity: " << table.getCapacity() << "\n";
    std::cout << "Final load factor: " << table.loadFactor() << "\n";
}

// Demo 5: Очистка DELETED ячеек
void demo5_DeleteCleaning() {
    printSection("5. CLEANUP OF DELETED CELLS");
    
    // Создаем таблицу и заполняем
    OpenAddressingHashTable<> table(11, 0.7);
    
    std::cout << "Inserting 20 elements...\n";
    for (int i = 1; i <= 20; ++i) {
        table.insert(i, i);
    }
    
    std::cout << "Size: " << table.size() 
              << " | Capacity: " << table.getCapacity() << "\n\n";
    
    // Удаляем много элементов
    std::cout << "Removing first 15 elements...\n";
    for (int i = 1; i <= 15; ++i) {
        table.remove(i);
    }
    
    std::cout << "After removal:\n";
    std::cout << "  Active: " << table.size() 
              << " | DELETED cells: " << table.getDeletedCount()
              << " | Capacity: " << table.getCapacity() << "\n\n";
    
    // Вставляем еще один элемент для триггера rehash
    std::cout << "Inserting 1 more element (triggers rehash)...\n";
    table.insert(100, 100);
    
    std::cout << "After rehash:\n";
    std::cout << "  Active: " << table.size() 
              << " | DELETED cells: " << table.getDeletedCount()
              << " | Capacity: " << table.getCapacity() << "\n\n";
    
    std::cout << "Result: DELETED cells cleaned, table compacted\n";
}

// Demo 6: Stress test
void demo6_StressTest() {
    printSection("6. STRESS TEST");
    
    // Создаем таблицу и добавляем много элементов
    OpenAddressingHashTable<> table(11, 0.7);
    
    std::cout << "Inserting 1000 elements...\n";
    for (int i = 0; i < 1000; ++i) {
        table.insert(i, i * 2);
    }
    
    std::cout << "Active: " << table.size() << "\n";
    std::cout << "Capacity: " << table.getCapacity() << "\n";
    std::cout << "Load factor: " << table.loadFactor() << "\n\n";
    
    // Проверяем несколько элементов
    std::cout << "Checking 100 random elements:\n";
    int errors = 0;
    for (int i = 0; i < 100; ++i) {
        int val;
        if (!table.search(i, val) || val != i * 2) {
            errors++;
        }
    }
    
    if (errors == 0) {
        std::cout << "Result: All elements found and correct\n";
    } else {
        std::cout << "Result: " << errors << " errors\n";
    }
}

int main() {
    printHeader("OPEN ADDRESSING HASH TABLE DEMONSTRATION");
    
    demo1_BasicOperations();
    demo2_LazyDeletion();
    demo3_QuadraticProbing();
    demo4_Rehashing();
    demo5_DeleteCleaning();
    demo6_StressTest();
    
    printHeader("END OF DEMONSTRATION");
    
    return 0;
}
