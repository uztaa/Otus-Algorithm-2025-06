#pragma once

#include "IHasher.hpp"
#include "StdHasher.hpp"
#include <vector>
#include <cstdint>
#include <cstddef>
#include <cmath>
#include <string>
#include <algorithm>
#include <stdexcept>

/**
 * @brief Bloom Filter — вероятностная структура для проверки принадлежности.
 * 
 * Гарантии:
 * - Если элемент был добавлен → contains() всегда вернёт true (нет False Negatives)
 * - Если элемент не был добавлен → contains() может вернуть true (False Positive)
 * 
 * Параметры:
 * - m: размер битового массива
 * - k: количество хэш-функций
 * - n: ожидаемое количество элементов
 * 
 * Вероятность False Positive: FPR ≈ (1 - e^(-kn/m))^k
 * 
 * Оптимальные параметры для заданного FPR:
 * - m = -n × ln(FPR) / (ln(2))²
 * - k = (m/n) × ln(2)
 */
class BloomFilter {
public:
    /**
     * @brief Конструктор с автоматическим расчётом параметров.
     * 
     * @param expectedElements Ожидаемое количество элементов (n)
     * @param falsePositiveRate Желаемая вероятность False Positive (0 < FPR < 1)
     * @param hasher Хэш-сервис (по умолчанию StdHasher)
     */
    BloomFilter(size_t expectedElements, double falsePositiveRate,
                HasherPtr hasher = std::make_shared<StdHasher>())
        : hasher_(hasher) {
        if (expectedElements == 0) {
            throw std::invalid_argument("expectedElements must be > 0");
        }
        if (falsePositiveRate <= 0.0 || falsePositiveRate >= 1.0) {
            throw std::invalid_argument("falsePositiveRate must be in (0, 1)");
        }
        
        numBits_ = optimalNumBits(expectedElements, falsePositiveRate);
        numHashes_ = optimalNumHashes(numBits_, expectedElements);
        
        numBits_ = std::max(numBits_, size_t(64));
        numHashes_ = std::max(numHashes_, size_t(1));
        
        size_t numWords = (numBits_ + 63) / 64;
        bits_.resize(numWords, 0);
    }
    
    /**
     * @brief Конструктор с явным указанием параметров.
     * 
     * @param numBits Размер битового массива (m)
     * @param numHashes Количество хэш-функций (k)
     * @param hasher Хэш-сервис
     */
    BloomFilter(size_t numBits, size_t numHashes,
                HasherPtr hasher)
        : hasher_(hasher) {
        if (numBits == 0) {
            throw std::invalid_argument("numBits must be > 0");
        }
        if (numHashes == 0) {
            throw std::invalid_argument("numHashes must be > 0");
        }
        
        numBits_ = std::max(numBits, size_t(64));
        numHashes_ = numHashes;
        
        size_t numWords = (numBits_ + 63) / 64;
        bits_.resize(numWords, 0);
    }
    
    /**
     * @brief Добавить элемент в фильтр (uint64_t).
     */
    void add(uint64_t element) {
        for (size_t i = 0; i < numHashes_; ++i) {
            uint64_t h = hasher_->hash(element, i);
            setBit(h % numBits_);
        }
        ++insertedCount_;
    }
    
    /**
     * @brief Добавить элемент в фильтр (string).
     */
    void add(const std::string& element) {
        for (size_t i = 0; i < numHashes_; ++i) {
            uint64_t h = hasher_->hash(element, i);
            setBit(h % numBits_);
        }
        ++insertedCount_;
    }
    
    /**
     * @brief Проверить, может ли элемент быть в фильтре (uint64_t).
     * @return false — элемента точно нет; true — элемент возможно есть
     */
    bool contains(uint64_t element) const {
        for (size_t i = 0; i < numHashes_; ++i) {
            uint64_t h = hasher_->hash(element, i);
            if (!getBit(h % numBits_)) {
                return false;
            }
        }
        return true;
    }
    
    /**
     * @brief Проверить, может ли элемент быть в фильтре (string).
     */
    bool contains(const std::string& element) const {
        for (size_t i = 0; i < numHashes_; ++i) {
            uint64_t h = hasher_->hash(element, i);
            if (!getBit(h % numBits_)) {
                return false;
            }
        }
        return true;
    }
    
    /**
     * @brief Очистить фильтр.
     */
    void clear() {
        std::fill(bits_.begin(), bits_.end(), 0);
        insertedCount_ = 0;
    }
    
    size_t getNumBits() const { return numBits_; }
    size_t getNumHashes() const { return numHashes_; }
    
    /**
     * @brief Получить количество установленных битов.
     */
    size_t getSetBitsCount() const {
        size_t count = 0;
        for (uint64_t word : bits_) {
#if defined(__GNUC__) || defined(__clang__)
            count += __builtin_popcountll(word);
#else
            while (word) {
                count += word & 1;
                word >>= 1;
            }
#endif
        }
        return count;
    }
    
    /**
     * @brief Оценить текущую вероятность False Positive.
     */
    double estimateFalsePositiveRate() const {
        double ratio = static_cast<double>(getSetBitsCount()) / numBits_;
        return std::pow(ratio, static_cast<double>(numHashes_));
    }
    
    size_t getMemoryUsage() const {
        return bits_.size() * sizeof(uint64_t) + sizeof(*this);
    }
    
    std::string getName() const { return "BloomFilter"; }
    std::string getHasherName() const { return hasher_->getName(); }

private:
    HasherPtr hasher_;
    size_t numBits_;
    size_t numHashes_;
    std::vector<uint64_t> bits_;
    size_t insertedCount_ = 0;
    
    void setBit(size_t index) {
        size_t wordIndex = index / 64;
        size_t bitIndex = index % 64;
        bits_[wordIndex] |= (1ULL << bitIndex);
    }
    
    bool getBit(size_t index) const {
        size_t wordIndex = index / 64;
        size_t bitIndex = index % 64;
        return (bits_[wordIndex] & (1ULL << bitIndex)) != 0;
    }
    
    static size_t optimalNumBits(size_t n, double fpr) {
        double ln2 = std::log(2.0);
        double m = -static_cast<double>(n) * std::log(fpr) / (ln2 * ln2);
        return static_cast<size_t>(std::ceil(m));
    }
    
    static size_t optimalNumHashes(size_t m, size_t n) {
        if (n == 0) return 1;
        double k = (static_cast<double>(m) / n) * std::log(2.0);
        return std::max(size_t(1), static_cast<size_t>(std::round(k)));
    }
};
