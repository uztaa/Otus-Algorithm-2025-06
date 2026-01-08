#pragma once

#include "IHasher.hpp"
#include "StdHasher.hpp"
#include <vector>
#include <cstdint>
#include <cstddef>
#include <string>
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <limits>

/**
 * @brief Count-Min Sketch — вероятностная структура для подсчёта частоты элементов.
 * 
 * Позволяет оценить, сколько раз каждый элемент встречался в потоке данных,
 * используя фиксированный объём памяти независимо от количества уникальных элементов.
 * 
 * Гарантии:
 * - estimate(x) >= actual_count(x) (никогда не занижает, может завышать из-за коллизий)
 * - С вероятностью >= 1-δ: estimate(x) <= actual_count(x) + ε × N
 * 
 * Параметры:
 * - ε (epsilon): максимальная относительная ошибка
 * - δ (delta): вероятность превышения ошибки
 * - w = ⌈e/ε⌉ (ширина)
 * - d = ⌈ln(1/δ)⌉ (глубина)
 */
class CountMinSketch {
public:
    /**
     * @brief Конструктор с параметрами точности.
     * 
     * @param epsilon Относительная ошибка (0 < ε < 1)
     * @param delta Вероятность превышения ошибки (0 < δ < 1)
     * @param hasher Хэш-сервис
     */
    CountMinSketch(double epsilon, double delta,
                   HasherPtr hasher = std::make_shared<StdHasher>())
        : hasher_(hasher) {
        if (epsilon <= 0.0 || epsilon >= 1.0) {
            throw std::invalid_argument("epsilon must be in (0, 1)");
        }
        if (delta <= 0.0 || delta >= 1.0) {
            throw std::invalid_argument("delta must be in (0, 1)");
        }
        
        // w = ⌈e/ε⌉
        width_ = static_cast<size_t>(std::ceil(E / epsilon));
        // d = ⌈ln(1/δ)⌉
        depth_ = static_cast<size_t>(std::ceil(std::log(1.0 / delta)));
        
        width_ = std::max(width_, size_t(1));
        depth_ = std::max(depth_, size_t(1));
        
        counters_.resize(depth_, std::vector<uint64_t>(width_, 0));
    }
    
    /**
     * @brief Конструктор с явным указанием размеров.
     * 
     * @param width Ширина массива (w)
     * @param depth Глубина (d)
     * @param hasher Хэш-сервис
     */
    CountMinSketch(size_t width, size_t depth,
                   HasherPtr hasher)
        : hasher_(hasher) {
        if (width == 0) {
            throw std::invalid_argument("width must be > 0");
        }
        if (depth == 0) {
            throw std::invalid_argument("depth must be > 0");
        }
        
        width_ = width;
        depth_ = depth;
        counters_.resize(depth_, std::vector<uint64_t>(width_, 0));
    }
    
    /**
     * @brief Добавить элемент (uint64_t).
     */
    void add(uint64_t element, uint64_t count = 1) {
        for (size_t i = 0; i < depth_; ++i) {
            uint64_t h = hasher_->hash(element, i);
            counters_[i][h % width_] += count;
        }
        totalCount_ += count;
    }
    
    /**
     * @brief Добавить элемент (string).
     */
    void add(const std::string& element, uint64_t count = 1) {
        for (size_t i = 0; i < depth_; ++i) {
            uint64_t h = hasher_->hash(element, i);
            counters_[i][h % width_] += count;
        }
        totalCount_ += count;
    }
    
    /**
     * @brief Оценить частоту элемента (uint64_t).
     * @return Оценка частоты (>= реального значения)
     */
    uint64_t estimate(uint64_t element) const {
        uint64_t minCount = std::numeric_limits<uint64_t>::max();
        for (size_t i = 0; i < depth_; ++i) {
            uint64_t h = hasher_->hash(element, i);
            minCount = std::min(minCount, counters_[i][h % width_]);
        }
        return minCount;
    }
    
    /**
     * @brief Оценить частоту элемента (string).
     */
    uint64_t estimate(const std::string& element) const {
        uint64_t minCount = std::numeric_limits<uint64_t>::max();
        for (size_t i = 0; i < depth_; ++i) {
            uint64_t h = hasher_->hash(element, i);
            minCount = std::min(minCount, counters_[i][h % width_]);
        }
        return minCount;
    }
    
    void clear() {
        for (auto& row : counters_) {
            std::fill(row.begin(), row.end(), 0);
        }
        totalCount_ = 0;
    }
    
    size_t getWidth() const { return width_; }
    size_t getDepth() const { return depth_; }
    uint64_t getTotalCount() const { return totalCount_; }
    
    size_t getMemoryUsage() const {
        return depth_ * width_ * sizeof(uint64_t) + sizeof(*this);
    }
    
    std::string getName() const { return "CountMinSketch"; }
    std::string getHasherName() const { return hasher_->getName(); }

private:
    static constexpr double E = 2.718281828459045;
    
    HasherPtr hasher_;
    size_t width_;
    size_t depth_;
    std::vector<std::vector<uint64_t>> counters_;
    uint64_t totalCount_ = 0;
};
