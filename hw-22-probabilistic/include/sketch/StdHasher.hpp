#pragma once

#include "IHasher.hpp"
#include <functional>

/**
 * @brief Реализация хэширования на основе std::hash.
 * 
 * Простая и переносимая реализация, но качество распределения
 * может быть хуже, чем у специализированных хэш-функций.
 * 
 * Используется по умолчанию для простоты.
 * Для production рекомендуется MurmurHasher.
 */
class StdHasher : public IHasher {
public:
    uint64_t hash(const void* data, size_t size, uint64_t seed = 0) const override {
        // Простая реализация: XOR байтов с seed
        const uint8_t* bytes = static_cast<const uint8_t*>(data);
        uint64_t h = seed;
        
        for (size_t i = 0; i < size; ++i) {
            h ^= static_cast<uint64_t>(bytes[i]) << ((i % 8) * 8);
            h *= 0x9e3779b97f4a7c15ULL;  // Golden ratio
            h ^= h >> 33;
        }
        
        return finalize(h);
    }
    
    uint64_t hash(uint64_t value, uint64_t seed = 0) const override {
        // splitmix64 — надёжная хэш-функция для целых чисел
        // std::hash<uint64_t> на многих платформах просто возвращает само число!
        uint64_t x = value + seed + 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        x = x ^ (x >> 31);
        return x;
    }
    
    uint64_t hash(const std::string& value, uint64_t seed = 0) const override {
        uint64_t h = std::hash<std::string>{}(value);
        // splitmix64 для финализации
        uint64_t x = h + seed + 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        x = x ^ (x >> 31);
        return x;
    }
    
    std::string getName() const override {
        return "std::hash";
    }

private:
    /**
     * @brief Смешивание хэша с seed.
     * 
     * ВАЖНО: seed=0 тоже должен влиять на результат!
     * Добавляем константу, чтобы h ^= ... всегда что-то делал.
     */
    static uint64_t mix(uint64_t h, uint64_t seed) {
        // Добавляем константу к seed, чтобы seed=0 тоже давал перемешивание
        h ^= (seed + 0x9e3779b97f4a7c15ULL);
        h *= 0x9e3779b97f4a7c15ULL;
        h ^= h >> 33;
        h *= 0xc4ceb9fe1a85ec53ULL;
        h ^= h >> 33;
        return h;
    }
    
    /**
     * @brief Финализация хэша (avalanche).
     */
    static uint64_t finalize(uint64_t h) {
        h ^= h >> 33;
        h *= 0xc4ceb9fe1a85ec53ULL;
        h ^= h >> 33;
        return h;
    }
};
