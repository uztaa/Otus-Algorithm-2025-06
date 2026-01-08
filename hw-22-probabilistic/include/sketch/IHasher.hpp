#pragma once

#include <cstdint>
#include <cstddef>
#include <memory>
#include <string>

/**
 * @brief Интерфейс хэш-сервиса для вероятностных структур данных.
 * 
 * Позволяет подменять реализации хэш-функций:
 * - StdHasher: использует std::hash (простой, переносимый)
 * - MurmurHasher: использует MurmurHash3 (лучшее распределение)
 * 
 * Для production-кода рекомендуется MurmurHash3 или xxHash,
 * так как они обеспечивают лучшее равномерное распределение битов.
 */
class IHasher {
public:
    virtual ~IHasher() = default;
    
    /**
     * @brief Вычислить 64-битный хэш.
     * @param data Указатель на данные
     * @param size Размер данных в байтах
     * @param seed Начальное значение (seed)
     * @return 64-битный хэш
     */
    virtual uint64_t hash(const void* data, size_t size, uint64_t seed = 0) const = 0;
    
    /**
     * @brief Вычислить хэш для целого числа.
     */
    virtual uint64_t hash(uint64_t value, uint64_t seed = 0) const = 0;
    
    /**
     * @brief Вычислить хэш для строки.
     */
    virtual uint64_t hash(const std::string& value, uint64_t seed = 0) const = 0;
    
    /**
     * @brief Получить название реализации.
     */
    virtual std::string getName() const = 0;
};

using HasherPtr = std::shared_ptr<IHasher>;
