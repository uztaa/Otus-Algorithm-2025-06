#pragma once

#include "IHasher.hpp"
#include <cstring>

/**
 * @brief Реализация хэширования на основе MurmurHash3.
 * 
 * MurmurHash3 — быстрая некриптографическая хэш-функция с отличным
 * равномерным распределением битов. Идеальна для вероятностных структур.
 * 
 * Особенности:
 * - Отличный avalanche effect (изменение 1 бита входа меняет ~50% битов выхода)
 * - Очень быстрая на современных процессорах
 * - Хорошо работает с короткими ключами
 * 
 * Оригинал: https://github.com/aappleby/smhasher
 * 
 * Эта реализация — 64-битная версия MurmurHash3_x64_128,
 * возвращающая младшие 64 бита.
 */
class MurmurHasher : public IHasher {
public:
    uint64_t hash(const void* data, size_t size, uint64_t seed = 0) const override {
        return murmurHash3_64(data, size, seed);
    }
    
    uint64_t hash(uint64_t value, uint64_t seed = 0) const override {
        return murmurHash3_64(&value, sizeof(value), seed);
    }
    
    uint64_t hash(const std::string& value, uint64_t seed = 0) const override {
        return murmurHash3_64(value.data(), value.size(), seed);
    }
    
    std::string getName() const override {
        return "MurmurHash3";
    }

private:
    /**
     * @brief MurmurHash3 64-bit finalizer.
     */
    static uint64_t fmix64(uint64_t k) {
        k ^= k >> 33;
        k *= 0xff51afd7ed558ccdULL;
        k ^= k >> 33;
        k *= 0xc4ceb9fe1a85ec53ULL;
        k ^= k >> 33;
        return k;
    }
    
    /**
     * @brief Rotate left.
     */
    static uint64_t rotl64(uint64_t x, int8_t r) {
        return (x << r) | (x >> (64 - r));
    }
    
    /**
     * @brief MurmurHash3_x64_128, возвращает младшие 64 бита.
     */
    static uint64_t murmurHash3_64(const void* key, size_t len, uint64_t seed) {
        const uint8_t* data = static_cast<const uint8_t*>(key);
        const size_t nblocks = len / 16;
        
        uint64_t h1 = seed;
        uint64_t h2 = seed;
        
        const uint64_t c1 = 0x87c37b91114253d5ULL;
        const uint64_t c2 = 0x4cf5ad432745937fULL;
        
        // Body
        const uint64_t* blocks = reinterpret_cast<const uint64_t*>(data);
        
        for (size_t i = 0; i < nblocks; ++i) {
            uint64_t k1 = getBlock64(blocks, i * 2 + 0);
            uint64_t k2 = getBlock64(blocks, i * 2 + 1);
            
            k1 *= c1;
            k1 = rotl64(k1, 31);
            k1 *= c2;
            h1 ^= k1;
            
            h1 = rotl64(h1, 27);
            h1 += h2;
            h1 = h1 * 5 + 0x52dce729;
            
            k2 *= c2;
            k2 = rotl64(k2, 33);
            k2 *= c1;
            h2 ^= k2;
            
            h2 = rotl64(h2, 31);
            h2 += h1;
            h2 = h2 * 5 + 0x38495ab5;
        }
        
        // Tail
        const uint8_t* tail = data + nblocks * 16;
        
        uint64_t k1 = 0;
        uint64_t k2 = 0;
        
        switch (len & 15) {
            case 15: k2 ^= static_cast<uint64_t>(tail[14]) << 48; [[fallthrough]];
            case 14: k2 ^= static_cast<uint64_t>(tail[13]) << 40; [[fallthrough]];
            case 13: k2 ^= static_cast<uint64_t>(tail[12]) << 32; [[fallthrough]];
            case 12: k2 ^= static_cast<uint64_t>(tail[11]) << 24; [[fallthrough]];
            case 11: k2 ^= static_cast<uint64_t>(tail[10]) << 16; [[fallthrough]];
            case 10: k2 ^= static_cast<uint64_t>(tail[ 9]) << 8;  [[fallthrough]];
            case  9: k2 ^= static_cast<uint64_t>(tail[ 8]) << 0;
                     k2 *= c2;
                     k2 = rotl64(k2, 33);
                     k2 *= c1;
                     h2 ^= k2;
                     [[fallthrough]];
                     
            case  8: k1 ^= static_cast<uint64_t>(tail[ 7]) << 56; [[fallthrough]];
            case  7: k1 ^= static_cast<uint64_t>(tail[ 6]) << 48; [[fallthrough]];
            case  6: k1 ^= static_cast<uint64_t>(tail[ 5]) << 40; [[fallthrough]];
            case  5: k1 ^= static_cast<uint64_t>(tail[ 4]) << 32; [[fallthrough]];
            case  4: k1 ^= static_cast<uint64_t>(tail[ 3]) << 24; [[fallthrough]];
            case  3: k1 ^= static_cast<uint64_t>(tail[ 2]) << 16; [[fallthrough]];
            case  2: k1 ^= static_cast<uint64_t>(tail[ 1]) << 8;  [[fallthrough]];
            case  1: k1 ^= static_cast<uint64_t>(tail[ 0]) << 0;
                     k1 *= c1;
                     k1 = rotl64(k1, 31);
                     k1 *= c2;
                     h1 ^= k1;
        }
        
        // Finalization
        h1 ^= len;
        h2 ^= len;
        
        h1 += h2;
        h2 += h1;
        
        h1 = fmix64(h1);
        h2 = fmix64(h2);
        
        h1 += h2;
        // h2 += h1;  // Не нужно, возвращаем только h1
        
        return h1;
    }
    
    /**
     * @brief Безопасное чтение 64-битного блока (для невыровненных данных).
     */
    static uint64_t getBlock64(const uint64_t* p, size_t i) {
        uint64_t result;
        std::memcpy(&result, reinterpret_cast<const char*>(p) + i * sizeof(uint64_t), sizeof(result));
        return result;
    }
};
