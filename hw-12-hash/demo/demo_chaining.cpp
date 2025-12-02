#include "ChainingHashTable.hpp"
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
    ChainingHashTable<SimpleHashFunction<>> table(11);
    
    std::cout << "Inserting: 42->100, 10->200, 37->370, 8->80\n";
    table.insert(42, 100);
    table.insert(10, 200);
    table.insert(37, 370);
    table.insert(8, 80);
    
    std::cout << "Size: " << table.size() 
              << " | Load factor: " << std::fixed << std::setprecision(3) 
              << table.loadFactor() << "\n\n";
    
    // Ищем ключи
    std::cout << "Search for key 42: ";
    int value = 0;
    if (table.search(42, value)) {
        std::cout << "FOUND, value = " << value << "\n";
    }
    
    std::cout << "Search for key 999: ";
    if (table.search(999, value)) {
        std::cout << "FOUND\n";
    } else {
        std::cout << "NOT FOUND\n";
    }
}

// Demo 2: Обновление значений
void demo2_UpdateValues() {
    printSection("2. UPDATE VALUES");
    
    // Создаем таблицу и вставляем элемент
    ChainingHashTable<SimpleHashFunction<>> table(11);
    
    std::cout << "Insert: key=5, value=50\n";
    table.insert(5, 50);
    
    // Проверяем значение
    int val = 0;
    table.search(5, val);
    std::cout << "Current value: " << val << "\n";
    
    // Обновляем
    std::cout << "\nUpdate: key=5, value=500\n";
    table.insert(5, 500);
    
    table.search(5, val);
    std::cout << "New value: " << val << "\n";
    std::cout << "Table size: " << table.size() << " (unchanged)\n";
}

// Demo 3: Удаление элементов
void demo3_RemoveElements() {
    printSection("3. REMOVE ELEMENTS");
    
    // Создаем таблицу с элементами
    ChainingHashTable<SimpleHashFunction<>> table(11);
    
    for (int i = 1; i <= 5; ++i) {
        table.insert(i, i * 100);
    }
    
    std::cout << "Inserted 5 elements. Size: " << table.size() << "\n\n";
    
    // Удаляем элемент
    std::cout << "Remove key 3: ";
    if (table.remove(3)) {
        std::cout << "SUCCESS\n";
    }
    std::cout << "Size: " << table.size() << "\n\n";
    
    // Проверяем удаленный ключ
    std::cout << "Search for deleted key 3: ";
    int val = 0;
    if (table.search(3, val)) {
        std::cout << "FOUND\n";
    } else {
        std::cout << "NOT FOUND (as expected)\n";
    }
}

// Demo 4: Автоматическое rehashing
void demo4_Rehashing() {
    printSection("4. AUTOMATIC REHASHING");
    
    // Создаем таблицу маленького размера
    ChainingHashTable<SimpleHashFunction<>> table(5);
    
    std::cout << "Initial capacity: " << table.getCapacity() << "\n";
    std::cout << "Load factor threshold: 0.75\n\n";
    
    // Вставляем элементы
    for (int i = 1; i <= 20; ++i) {
        int old_capacity = table.getCapacity();
        table.insert(i, i * 10);
        
        // Если размер изменился, то был rehash
        if (table.getCapacity() != old_capacity) {
            std::cout << "REHASH: " << old_capacity << " -> " 
                      << table.getCapacity() << "\n";
        }
    }
    
    std::cout << "\nFinal capacity: " << table.getCapacity() << "\n";
    std::cout << "Final load factor: " << table.loadFactor() << "\n";
    
    // Проверяем что все элементы на месте
    std::cout << "\nVerification: all elements found: ";
    bool all_found = true;
    for (int i = 1; i <= 20; ++i) {
        int val = 0;
        if (!table.search(i, val) || val != i * 10) {
            all_found = false;
            break;
        }
    }
    std::cout << (all_found ? "YES\n" : "NO\n");
}

// Demo 5: Разрешение коллизий
void demo5_CollisionHandling() {
    printSection("5. COLLISION HANDLING");
    
    // Создаем маленькую таблицу для коллизий
    ChainingHashTable<SimpleHashFunction<>> table(7);
    
    std::cout << "Inserting elements (collisions with modulo 7):\n";
    std::cout << "  7 mod 7 = 0\n";
    std::cout << "  14 mod 7 = 0\n";
    std::cout << "  21 mod 7 = 0\n";
    std::cout << "All three indices are same -> one chain!\n\n";
    
    // Вставляем элементы которые коллизируют
    table.insert(7, 700);
    table.insert(14, 1400);
    table.insert(21, 2100);
    
    std::cout << "Table size: " << table.size() << "\n";
    std::cout << "All elements found:\n";
    
    int val = 0;
    if (table.search(7, val)) std::cout << "  7 -> " << val << "\n";
    if (table.search(14, val)) std::cout << "  14 -> " << val << "\n";
    if (table.search(21, val)) std::cout << "  21 -> " << val << "\n";
}

// Demo 6: Разные хеш-функции
void demo6_DifferentHashFunctions() {
    printSection("6. DIFFERENT HASH FUNCTIONS");
    
    std::cout << "Comparing hash functions for key=12345, capacity=11:\n\n";
    
    int key = 12345;
    int capacity = 11;
    
    SimpleHashFunction<> simple;
    MultiplicativeHashFunction<> multiplicative;
    BitwiseHashFunction<> bitwise;
    JavaLikeHashFunction<> java_like;
    
    std::cout << "Simple:          " << simple(key, capacity) << "\n";
    std::cout << "Multiplicative:  " << multiplicative(key, capacity) << "\n";
    std::cout << "Bitwise:         " << bitwise(key, capacity) << "\n";
    std::cout << "Java-like:       " << java_like(key, capacity) << "\n";
}

int main() {
    printHeader("CHAINING HASH TABLE DEMONSTRATION");
    
    demo1_BasicOperations();
    demo2_UpdateValues();
    demo3_RemoveElements();
    demo4_Rehashing();
    demo5_CollisionHandling();
    demo6_DifferentHashFunctions();
    
    printHeader("END OF DEMONSTRATION");
    
    return 0;
}
