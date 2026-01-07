#pragma once

#include <vector>
#include <cstdint>
#include <string>

/**
 * @brief Интерфейс алгоритма сжатия данных.
 * 
 * Использование:
 * @code
 * auto compressor = std::make_unique<RLECompressor>();
 * 
 * std::vector<uint8_t> original = {1, 1, 1, 2, 2, 3};
 * auto compressed = compressor->compress(original);
 * auto decompressed = compressor->decompress(compressed);
 * 
 * assert(original == decompressed);
 * @endcode
 */
class ICompressor {
public:
    virtual ~ICompressor() = default;
    
    /**
     * @brief Сжать данные.
     * @param data Исходные данные
     * @return Сжатые данные
     */
    virtual std::vector<uint8_t> compress(const std::vector<uint8_t>& data) = 0;
    
    /**
     * @brief Распаковать данные.
     * @param data Сжатые данные
     * @return Исходные данные
     */
    virtual std::vector<uint8_t> decompress(const std::vector<uint8_t>& data) = 0;
    
    /**
     * @brief Получить название алгоритма.
     * @return Название алгоритма
     */
    virtual std::string getName() const = 0;
    
    /**
     * @brief Рассчитать коэффициент сжатия.
     * @param originalSize Размер исходных данных
     * @param compressedSize Размер сжатых данных
     * @return Коэффициент сжатия (>1 означает сжатие, <1 — раздувание)
     */
    static double compressionRatio(size_t originalSize, size_t compressedSize) {
        if (compressedSize == 0) return 0.0;
        return static_cast<double>(originalSize) / compressedSize;
    }
    
    /**
     * @brief Рассчитать степень сжатия в процентах.
     * @param originalSize Размер исходных данных
     * @param compressedSize Размер сжатых данных
     * @return Степень сжатия (положительное — сжатие, отрицательное — раздувание)
     */
    static double compressionPercent(size_t originalSize, size_t compressedSize) {
        if (originalSize == 0) return 0.0;
        return (1.0 - static_cast<double>(compressedSize) / originalSize) * 100.0;
    }
};
