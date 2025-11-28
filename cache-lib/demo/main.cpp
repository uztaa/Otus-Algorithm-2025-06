#include <cache/Cache.hpp>
#include <cache/policies/LRUPolicy.hpp>
#include <cache/listeners/LoggingListener.hpp>
#include <cache/listeners/StatsListener.hpp>

#include <iostream>
#include <string>

/**
 * @brief Демонстрация возможностей библиотеки кэширования
 * 
 * Показываем:
 * - Создание кэша с LRU политикой
 * - Базовые операции (put, get, remove, clear)
 * - Работу слушателей (логирование, статистика)
 * - Вытеснение при переполнении
 * - Смену политики в runtime
 */

void printSeparator(const std::string& title) {
    std::cout << "\n========== " << title << " ==========\n\n";
}

int main() {
    std::cout << "=== Cache Library Demo ===\n";

    // --------------------------------------------------
    // 1. Базовое использование
    // --------------------------------------------------
    printSeparator("1. Basic usage");
    
    // Создаём кэш на 3 элемента с LRU политикой
    Cache<std::string, int> cache(3, std::make_unique<LRUPolicy<std::string>>());
    
    std::cout << "Created cache with capacity: " << cache.capacity() << "\n\n";
    
    // Добавляем элементы
    cache.put("apple", 100);
    cache.put("banana", 200);
    cache.put("cherry", 300);
    
    std::cout << "Added 3 items. Size: " << cache.size() << "\n";
    
    // Получаем значение
    if (auto value = cache.get("banana")) {
        std::cout << "banana = " << value.value() << "\n";
    }
    
    // Проверяем наличие
    std::cout << "contains('apple'): " << (cache.contains("apple") ? "yes" : "no") << "\n";
    std::cout << "contains('mango'): " << (cache.contains("mango") ? "yes" : "no") << "\n";

    // --------------------------------------------------
    // 2. Вытеснение (Eviction)
    // --------------------------------------------------
    printSeparator("2. Eviction");
    
    // Кэш полон (3 элемента). Добавляем четвёртый — LRU вытеснится.
    // Порядок использования: banana (только что get), cherry, apple
    // apple — LRU, он вытеснится
    
    std::cout << "Cache is full. Adding 'durian'...\n";
    cache.put("durian", 400);
    
    std::cout << "After adding 'durian':\n";
    std::cout << "  contains('apple'): " << (cache.contains("apple") ? "yes" : "no") << " (evicted)\n";
    std::cout << "  contains('banana'): " << (cache.contains("banana") ? "yes" : "no") << "\n";
    std::cout << "  contains('cherry'): " << (cache.contains("cherry") ? "yes" : "no") << "\n";
    std::cout << "  contains('durian'): " << (cache.contains("durian") ? "yes" : "no") << "\n";

    // --------------------------------------------------
    // 3. Слушатели (Listeners)
    // --------------------------------------------------
    printSeparator("3. Listeners");
    
    // Создаём новый кэш со слушателями
    Cache<std::string, int> cacheWithListeners(3, std::make_unique<LRUPolicy<std::string>>());
    
    // Добавляем логгер
    auto logger = std::make_shared<LoggingListener<std::string, int>>("MyCache");
    cacheWithListeners.addListener(logger);
    
    // Добавляем сборщик статистики
    auto stats = std::make_shared<StatsListener<std::string, int>>();
    cacheWithListeners.addListener(stats);
    
    std::cout << "Performing operations with listeners attached:\n\n";
    
    cacheWithListeners.put("x", 10);
    cacheWithListeners.put("y", 20);
    cacheWithListeners.get("x");           // Hit
    cacheWithListeners.get("z");           // Miss
    cacheWithListeners.put("x", 15);       // Update
    cacheWithListeners.put("z", 30);
    cacheWithListeners.put("w", 40);       // Evicts "y"
    
    std::cout << "\nStatistics:\n";
    std::cout << "  Hits: " << stats->hits() << "\n";
    std::cout << "  Misses: " << stats->misses() << "\n";
    std::cout << "  Hit rate: " << (stats->hitRate() * 100) << "%\n";
    std::cout << "  Inserts: " << stats->inserts() << "\n";
    std::cout << "  Updates: " << stats->updates() << "\n";
    std::cout << "  Evictions: " << stats->evictions() << "\n";

    // --------------------------------------------------
    // 4. Удаление слушателя
    // --------------------------------------------------
    printSeparator("4. Remove listener");
    
    std::cout << "Removing logger, keeping stats...\n\n";
    cacheWithListeners.removeListener(logger);
    
    cacheWithListeners.put("new_key", 999);
    std::cout << "(No log output above)\n";
    std::cout << "Inserts after removing logger: " << stats->inserts() << "\n";

    // --------------------------------------------------
    // 5. Смена политики в runtime
    // --------------------------------------------------
    printSeparator("5. Change policy at runtime");
    
    Cache<int, std::string> numericCache(3, std::make_unique<LRUPolicy<int>>());
    
    numericCache.put(1, "one");
    numericCache.put(2, "two");
    numericCache.put(3, "three");
    
    std::cout << "Before policy change:\n";
    std::cout << "  Size: " << numericCache.size() << "\n";
    std::cout << "  get(1): " << numericCache.get(1).value_or("not found") << "\n";
    
    // Меняем политику — данные сохраняются, порядок сбрасывается
    std::cout << "\nChanging to new LRU policy...\n";
    numericCache.setEvictionPolicy(std::make_unique<LRUPolicy<int>>());
    
    std::cout << "After policy change:\n";
    std::cout << "  Size: " << numericCache.size() << " (data preserved)\n";
    std::cout << "  get(1): " << numericCache.get(1).value_or("not found") << "\n";
    std::cout << "  get(2): " << numericCache.get(2).value_or("not found") << "\n";
    std::cout << "  get(3): " << numericCache.get(3).value_or("not found") << "\n";

    // --------------------------------------------------
    // 6. Clear и Remove
    // --------------------------------------------------
    printSeparator("6. Remove and Clear");
    
    Cache<std::string, int> tempCache(10, std::make_unique<LRUPolicy<std::string>>());
    tempCache.put("a", 1);
    tempCache.put("b", 2);
    tempCache.put("c", 3);
    
    std::cout << "Initial size: " << tempCache.size() << "\n";
    
    bool removed = tempCache.remove("b");
    std::cout << "remove('b'): " << (removed ? "success" : "not found") << "\n";
    std::cout << "Size after remove: " << tempCache.size() << "\n";
    
    removed = tempCache.remove("nonexistent");
    std::cout << "remove('nonexistent'): " << (removed ? "success" : "not found") << "\n";
    
    tempCache.clear();
    std::cout << "Size after clear: " << tempCache.size() << "\n";

    // --------------------------------------------------
    std::cout << "\n=== Demo complete ===\n";
    
    return 0;
}