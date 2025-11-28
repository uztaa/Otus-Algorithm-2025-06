# Cache Library

Модульная библиотека кэширования на C++17 с поддержкой сменных политик вытеснения.

## Сборка
```bash
mkdir build && cd build
cmake ..
cmake --build .
ctest --output-on-failure
```

## Использование
```cpp
#include <cache/Cache.hpp>
#include <cache/policies/LRUPolicy.hpp>

int main() {
    auto cache = Cache<std::string, int>(
        1000,
        std::make_unique<LRUPolicy<std::string>>()
    );
    
    cache.put("key1", 42);
    auto value = cache.get("key1");
    
    return 0;
}
```