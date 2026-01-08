#pragma once

#include "IHasher.hpp"
#include "StdHasher.hpp"
#include "HashUtils.hpp"
#include <vector>
#include <cstdint>
#include <cstddef>
#include <string>
#include <cmath>
#include <algorithm>
#include <stdexcept>

/**
 * @brief HyperLogLog — вероятностная структура для подсчёта уникальных элементов.
 * 
 * Позволяет оценить cardinality (количество уникальных элементов) в потоке
 * с использованием минимальной памяти.
 * 
 * Идея алгоритма:
 * 1. Хэшируем каждый элемент в 64-битное число
 * 2. Первые p бит определяют номер "корзины" (регистра)
 * 3. Остальные биты используем для подсчёта позиции первой единицы
 * 4. В каждом регистре храним максимальную позицию первой единицы
 * 5. Используем среднее гармоническое для оценки cardinality
 * 
 * | p  | Регистров (m) | Память | Ошибка (σ) |
 * |----|---------------|--------|------------|
 * | 10 | 1024          | 5 KB   | 3.25%      |
 * | 12 | 4096          | 20 KB  | 1.6%       |
 * | 14 | 16384         | 82 KB  | 0.81%      |
 */
class HyperLogLog {
public:
    /**
     * @brief Конструктор.
     * 
     * @param precision Количество бит для индекса корзины (4 ≤ p ≤ 18)
     * @param hasher Хэш-сервис
     */
    explicit HyperLogLog(uint8_t precision = 14,
                         HasherPtr hasher = std::make_shared<StdHasher>())
        : hasher_(hasher), precision_(precision) {
        if (precision < 4 || precision > 18) {
            throw std::invalid_argument("precision must be in [4, 18]");
        }
        
        numRegisters_ = size_t(1) << precision;
        registers_.resize(numRegisters_, 0);
    }
    
    /**
     * @brief Добавить элемент (uint64_t).
     */
    void add(uint64_t element) {
        uint64_t hash = hasher_->hash(element, 0);
        addHash(hash);
    }
    
    /**
     * @brief Добавить элемент (string).
     */
    void add(const std::string& element) {
        uint64_t hash = hasher_->hash(element, 0);
        addHash(hash);
    }
    
    /**
     * @brief Оценить количество уникальных элементов.
     */
    uint64_t estimate() const {
        double raw = rawEstimate();
        
        // Коррекция для малых значений
        if (raw <= LINEAR_COUNTING_THRESHOLD * numRegisters_) {
            size_t zeroRegs = countZeroRegisters();
            if (zeroRegs > 0) {
                double lc = linearCounting(zeroRegs);
                return static_cast<uint64_t>(std::round(lc));
            }
        }
        
        // Коррекция для очень больших значений
        if (raw > LARGE_RANGE_THRESHOLD) {
            double corrected = -TWO_POW_32 * std::log(1.0 - raw / TWO_POW_32);
            return static_cast<uint64_t>(std::round(corrected));
        }
        
        return static_cast<uint64_t>(std::round(raw));
    }
    
    /**
     * @brief Объединить с другим HyperLogLog.
     */
    void merge(const HyperLogLog& other) {
        if (precision_ != other.precision_) {
            throw std::invalid_argument("Cannot merge HyperLogLog with different precision");
        }
        
        for (size_t i = 0; i < numRegisters_; ++i) {
            registers_[i] = std::max(registers_[i], other.registers_[i]);
        }
    }
    
    void clear() {
        std::fill(registers_.begin(), registers_.end(), 0);
    }
    
    uint8_t getPrecision() const { return precision_; }
    size_t getNumRegisters() const { return numRegisters_; }
    
    double getStandardError() const {
        return 1.04 / std::sqrt(static_cast<double>(numRegisters_));
    }
    
    size_t getMemoryUsage() const {
        return registers_.size() * sizeof(uint8_t) + sizeof(*this);
    }
    
    std::string getName() const { return "HyperLogLog"; }
    std::string getHasherName() const { return hasher_->getName(); }

private:
    static constexpr double LINEAR_COUNTING_THRESHOLD = 2.5;
    static constexpr double LARGE_RANGE_THRESHOLD = 143165576.533;
    static constexpr double TWO_POW_32 = 4294967296.0;
    
    HasherPtr hasher_;
    uint8_t precision_;
    size_t numRegisters_;
    std::vector<uint8_t> registers_;
    
    void addHash(uint64_t hash) {
        // Первые p бит → индекс корзины
        size_t index = hash >> (64 - precision_);
        
        // Остальные (64 - p) бит → позиция первой единицы
        uint64_t w = hash << precision_;
        int rank = HashUtils::firstOnePosition(w, 64 - precision_);
        
        if (rank > registers_[index]) {
            registers_[index] = static_cast<uint8_t>(rank);
        }
    }
    
    double getAlpha() const {
        switch (numRegisters_) {
            case 16: return 0.673;
            case 32: return 0.697;
            case 64: return 0.709;
            default: return 0.7213 / (1.0 + 1.079 / numRegisters_);
        }
    }
    
    size_t countZeroRegisters() const {
        size_t count = 0;
        for (uint8_t reg : registers_) {
            if (reg == 0) ++count;
        }
        return count;
    }
    
    double linearCounting(size_t zeroRegisters) const {
        return numRegisters_ * std::log(static_cast<double>(numRegisters_) / zeroRegisters);
    }
    
    double rawEstimate() const {
        double sum = 0.0;
        for (uint8_t reg : registers_) {
            sum += std::pow(2.0, -static_cast<double>(reg));
        }
        double Z = 1.0 / sum;
        return getAlpha() * numRegisters_ * numRegisters_ * Z;
    }
};
