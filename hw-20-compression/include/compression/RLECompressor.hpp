#pragma once

#include "ICompressor.hpp"
#include <vector>
#include <cstdint>
#include <string>

/**
 * @brief Базовый алгоритм Run-Length Encoding.
 * 
 * Формат сжатых данных:
 * - Каждая серия кодируется парой (count, byte)
 * - count ∈ [1, 255] — количество повторов
 * - byte — повторяющийся байт
 * 
 * Пример:
 * - Вход:  AAABBC (6 байт)
 * - Выход: 03 41 02 42 01 43 (6 байт)
 * 
 * Недостатки:
 * - Данные без повторов раздуваются в 2 раза
 * - Минимальный размер сжатого блока = 2 байта
 * 
 * Сложность: O(n) для сжатия и распаковки
 */
class RLECompressor : public ICompressor {
public:
    RLECompressor() = default;
    
    std::vector<uint8_t> compress(const std::vector<uint8_t>& data) override;
    std::vector<uint8_t> decompress(const std::vector<uint8_t>& data) override;
    std::string getName() const override;
};
