#pragma once

#include "ICompressor.hpp"
#include <vector>
#include <cstdint>
#include <string>

/**
 * @brief Улучшенный RLE на основе алгоритма PackBits.
 * 
 * Стандартный алгоритм из TIFF/MacPaint, решает проблему
 * раздувания данных без повторов.
 * 
 * Формат сжатых данных (флаговый байт N):
 * - N ∈ [0, 127]: следующие (N+1) байт — литералы (копируем как есть)
 * - N ∈ [-127, -1] (т.е. 129-255): следующий байт повторяется (1-N) раз
 * - N = -128 (т.е. 128): NOP, пропускается
 * 
 * Примеры:
 * - 0x00 0x41         → "A"           (1 литерал)
 * - 0x02 0x41 0x42 0x43 → "ABC"       (3 литерала)
 * - 0xFE 0x41         → "AAA"         (3 повтора, т.к. 1 - (-2) = 3)
 * - 0x81 0x41         → "AAAA...A"    (128 повторов)
 * 
 * Преимущества над базовым RLE:
 * - Не раздувает данные без повторов
 * - Гарантированный worst-case: размер ≤ original + original/128
 * 
 * Сложность: O(n) для сжатия и распаковки
 */
class PackBitsCompressor : public ICompressor {
public:
    PackBitsCompressor() = default;
    
    std::vector<uint8_t> compress(const std::vector<uint8_t>& data) override;
    std::vector<uint8_t> decompress(const std::vector<uint8_t>& data) override;
    std::string getName() const override;

private:
    /// Минимальная длина серии для кодирования как повтор
    static constexpr size_t MIN_RUN_LENGTH = 3;
    
    /// Максимальная длина блока литералов (N+1, где N ∈ [0,127])
    static constexpr size_t MAX_LITERAL_LENGTH = 128;
    
    /// Максимальная длина серии повторов (1-N, где N ∈ [-127,-1])
    static constexpr size_t MAX_RUN_LENGTH = 128;
    
    /**
     * @brief Записать блок литералов.
     */
    void flushLiterals(std::vector<uint8_t>& result, 
                       std::vector<uint8_t>& literals);
};
