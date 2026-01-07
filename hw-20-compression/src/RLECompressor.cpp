#include "compression/RLECompressor.hpp"
#include <stdexcept>

std::string RLECompressor::getName() const {
    return "RLE";
}

std::vector<uint8_t> RLECompressor::compress(const std::vector<uint8_t>& data) {
    std::vector<uint8_t> result;
    
    if (data.empty()) {
        return result;
    }
    
    size_t i = 0;
    while (i < data.size()) {
        uint8_t current = data[i];
        uint8_t count = 1;
        
        // Считаем длину серии (максимум 255)
        while (i + count < data.size() && 
               data[i + count] == current && 
               count < 255) {
            ++count;
        }
        
        // Записываем пару (count, byte)
        result.push_back(count);
        result.push_back(current);
        
        i += count;
    }
    
    return result;
}

std::vector<uint8_t> RLECompressor::decompress(const std::vector<uint8_t>& data) {
    std::vector<uint8_t> result;
    constexpr size_t MAX_OUTPUT_SIZE = 100 * 1024 * 1024;  // 100 MB лимит
    
    // Проверка: данные должны быть парами (count, byte)
    if (data.size() % 2 != 0) {
        throw std::runtime_error("Corrupted RLE data: odd number of bytes");
    }
    
    for (size_t i = 0; i < data.size(); i += 2) {
        uint8_t count = data[i];
        uint8_t byte = data[i + 1];
        
        // Проверка на нулевой count (невалидно)
        if (count == 0) {
            throw std::runtime_error("Corrupted RLE data: zero count");
        }
        
        // Проверка на декомпрессионную бомбу
        if (result.size() + count > MAX_OUTPUT_SIZE) {
            throw std::runtime_error("Decompression bomb detected");
        }
        
        // Добавляем count копий byte
        for (uint8_t j = 0; j < count; ++j) {
            result.push_back(byte);
        }
    }
    
    return result;
}
