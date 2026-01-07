#include "compression/PackBitsCompressor.hpp"
#include <algorithm>
#include <stdexcept>

std::string PackBitsCompressor::getName() const {
    return "PackBits";
}

void PackBitsCompressor::flushLiterals(std::vector<uint8_t>& result,
                                        std::vector<uint8_t>& literals) {
    while (!literals.empty()) {
        // Записываем блоками до 128 байт
        size_t count = std::min(literals.size(), MAX_LITERAL_LENGTH);
        
        // Флаг = count - 1 (диапазон 0-127 означает 1-128 литералов)
        result.push_back(static_cast<uint8_t>(count - 1));
        
        // Копируем литералы
        for (size_t j = 0; j < count; ++j) {
            result.push_back(literals[j]);
        }
        
        literals.erase(literals.begin(), literals.begin() + count);
    }
}

std::vector<uint8_t> PackBitsCompressor::compress(const std::vector<uint8_t>& data) {
    std::vector<uint8_t> result;
    std::vector<uint8_t> literals;
    
    if (data.empty()) {
        return result;
    }
    
    size_t i = 0;
    while (i < data.size()) {
        // Ищем серию повторов
        size_t runLen = 1;
        while (i + runLen < data.size() && 
               data[i + runLen] == data[i] && 
               runLen < MAX_RUN_LENGTH) {
            ++runLen;
        }
        
        if (runLen >= MIN_RUN_LENGTH) {
            // Серия достаточно длинная — кодируем как повтор
            
            // Сначала сбрасываем накопленные литералы
            flushLiterals(result, literals);
            
            // Флаг = 1 - runLen (диапазон -2..-127 означает 3-128 повторов)
            // В uint8_t: 254..129
            result.push_back(static_cast<uint8_t>(1 - static_cast<int>(runLen)));
            result.push_back(data[i]);
            
            i += runLen;
        } else {
            // Серия короткая — добавляем к литералам
            literals.push_back(data[i]);
            ++i;
        }
    }
    
    // Сбрасываем оставшиеся литералы
    flushLiterals(result, literals);
    
    return result;
}

std::vector<uint8_t> PackBitsCompressor::decompress(const std::vector<uint8_t>& data) {
    std::vector<uint8_t> result;
    constexpr size_t MAX_OUTPUT_SIZE = 100 * 1024 * 1024;  // 100 MB лимит
    
    size_t i = 0;
    while (i < data.size()) {
        int8_t flag = static_cast<int8_t>(data[i]);
        ++i;
        
        if (flag == -128) {
            continue;
        } else if (flag >= 0) {
            size_t count = static_cast<size_t>(flag) + 1;
            if (i + count > data.size()) {
                throw std::runtime_error("Corrupted data: unexpected end of literals");
            }
            if (result.size() + count > MAX_OUTPUT_SIZE) {
                throw std::runtime_error("Decompression bomb detected");
            }
            for (size_t j = 0; j < count; ++j) {
                result.push_back(data[i++]);
            }
        } else {
            size_t count = static_cast<size_t>(1 - flag);
            if (i >= data.size()) {
                throw std::runtime_error("Corrupted data: missing repeat byte");
            }
            if (result.size() + count > MAX_OUTPUT_SIZE) {
                throw std::runtime_error("Decompression bomb detected");
            }
            uint8_t byte = data[i++];
            for (size_t j = 0; j < count; ++j) {
                result.push_back(byte);
            }
        }
    }
    
    return result;
}
