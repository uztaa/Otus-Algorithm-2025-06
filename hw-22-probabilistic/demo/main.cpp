#include <iostream>
#include <iomanip>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <random>
#include <chrono>
#include <string>
#include <cmath>
#include <algorithm>
#include <cstdint>
#include <limits>

#include "sketch/BloomFilter.hpp"
#include "sketch/HyperLogLog.hpp"
#include "sketch/CountMinSketch.hpp"
#include "sketch/MurmurHasher.hpp"

// ============================================
// Генераторы тестовых данных
// ============================================

std::vector<uint64_t> generateUniqueIds(size_t count, unsigned int seed = 42) {
    std::vector<uint64_t> ids;
    ids.reserve(count);
    
    // Генерируем случайные уникальные числа (не последовательные!)
    // Последовательные числа могут давать артефакты в хэш-функциях
    std::unordered_set<uint64_t> seen;
    std::mt19937_64 rng(seed);
    const uint64_t maxVal = std::numeric_limits<uint64_t>::max();
    std::uniform_int_distribution<uint64_t> dist(0, maxVal / 2);
    
    while (ids.size() < count) {
        uint64_t val = dist(rng);
        if (seen.insert(val).second) {
            ids.push_back(val);
        }
    }
    
    return ids;
}

std::vector<uint64_t> generateWithDuplicates(size_t uniqueCount, size_t totalCount, unsigned int seed = 42) {
    std::vector<uint64_t> data;
    data.reserve(totalCount);
    std::mt19937_64 rng(seed);
    std::uniform_int_distribution<uint64_t> dist(0, uniqueCount - 1);
    for (size_t i = 0; i < totalCount; ++i) {
        data.push_back(dist(rng));
    }
    return data;
}

std::vector<uint64_t> generateZipfDistribution(size_t uniqueCount, size_t totalCount, 
                                                 double exponent = 1.0, unsigned int seed = 42) {
    std::vector<double> cdf(uniqueCount);
    double sum = 0.0;
    for (size_t i = 0; i < uniqueCount; ++i) {
        sum += 1.0 / std::pow(i + 1, exponent);
        cdf[i] = sum;
    }
    for (double& p : cdf) {
        p /= sum;
    }
    
    std::vector<uint64_t> data;
    data.reserve(totalCount);
    std::mt19937_64 rng(seed);
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    
    for (size_t i = 0; i < totalCount; ++i) {
        double r = dist(rng);
        auto it = std::lower_bound(cdf.begin(), cdf.end(), r);
        size_t index = std::distance(cdf.begin(), it);
        data.push_back(index);
    }
    return data;
}

void printHeader(const std::string& title) {
    std::cout << "\n";
    std::cout << "============================================" << std::endl;
    std::cout << "  " << title << std::endl;
    std::cout << "============================================" << std::endl;
}

// ============================================
// Бенчмарк Bloom Filter
// ============================================

void benchmarkBloomFilter() {
    printHeader("Bloom Filter vs std::unordered_set");
    
    const size_t n = 1000000;
    const double targetFpr = 0.01;
    const int numTrials = 5;
    const int testCount = 100000;
    
    std::cout << "\nПараметры:" << std::endl;
    std::cout << "  n (элементов): " << n << std::endl;
    std::cout << "  Целевой FPR: " << targetFpr * 100 << "%" << std::endl;
    std::cout << "  Методика: " << numTrials << " прогонов по " << testCount << " случайных запросов" << std::endl;
    
    auto ids = generateUniqueIds(n);
    
    auto stdHasher = std::make_shared<StdHasher>();
    auto murmurHasher = std::make_shared<MurmurHasher>();
    
    BloomFilter bfStd(n, targetFpr, stdHasher);
    BloomFilter bfMurmur(n, targetFpr, murmurHasher);
    
    for (uint64_t id : ids) {
        bfStd.add(id);
        bfMurmur.add(id);
    }
    
    // Проверка FP — генерируем случайные числа с ДРУГИМ seed (не пересекаются с добавленными)
    auto checkFP = [&](BloomFilter& bf, unsigned int seed) {
        int falsePositives = 0;
        std::mt19937_64 rng(seed + 1000);  // Другой seed чтобы не пересекаться
        const uint64_t maxVal = std::numeric_limits<uint64_t>::max();
        std::uniform_int_distribution<uint64_t> dist(maxVal / 2 + 1, maxVal);  // Другой диапазон!
        
        for (int i = 0; i < testCount; ++i) {
            uint64_t testValue = dist(rng);
            if (bf.contains(testValue)) {
                ++falsePositives;
            }
        }
        return static_cast<double>(falsePositives) / testCount;
    };
    
    double fprStd = 0, fprMurmur = 0;
    for (int trial = 0; trial < numTrials; ++trial) {
        fprStd += checkFP(bfStd, 100 + trial);
        fprMurmur += checkFP(bfMurmur, 100 + trial);
    }
    fprStd /= numTrials;
    fprMurmur /= numTrials;
    
    // unordered_set
    std::unordered_set<uint64_t> exactSet;
    for (uint64_t id : ids) {
        exactSet.insert(id);
    }
    size_t setMemory = exactSet.bucket_count() * sizeof(void*) + exactSet.size() * sizeof(uint64_t);
    
    std::cout << "\nРезультаты:" << std::endl;
    std::cout << "| Метрика               | std::hash       | MurmurHash3     | unordered_set   |" << std::endl;
    std::cout << "|-----------------------|-----------------|-----------------|-----------------|" << std::endl;
    std::cout << "| Память (байт)         | " << std::setw(15) << bfStd.getMemoryUsage() 
              << " | " << std::setw(15) << bfMurmur.getMemoryUsage()
              << " | " << std::setw(15) << setMemory << " |" << std::endl;
    std::cout << "| FPR (факт)            | " << std::setw(14) << std::fixed << std::setprecision(4) << fprStd * 100 << "%" 
              << " | " << std::setw(14) << fprMurmur * 100 << "%"
              << " | " << std::setw(15) << "0%" << " |" << std::endl;
    std::cout << "| FPR (ожидание)        | " << std::setw(14) << targetFpr * 100 << "%" 
              << " | " << std::setw(14) << targetFpr * 100 << "%"
              << " | " << std::setw(15) << "0%" << " |" << std::endl;
    
    double memorySavingStd = static_cast<double>(setMemory) / bfStd.getMemoryUsage();
    double memorySavingMurmur = static_cast<double>(setMemory) / bfMurmur.getMemoryUsage();
    std::cout << "\nЭкономия памяти: " << std::fixed << std::setprecision(1) 
              << memorySavingStd << "x (std::hash), " 
              << memorySavingMurmur << "x (MurmurHash3)" << std::endl;
}

// ============================================
// Бенчмарк HyperLogLog
// ============================================

void benchmarkHyperLogLog() {
    printHeader("HyperLogLog vs std::unordered_set");
    
    const size_t uniqueCount = 1000000;
    const size_t totalCount = 5000000;
    
    std::cout << "\nПараметры:" << std::endl;
    std::cout << "  Уникальных элементов: " << uniqueCount << std::endl;
    std::cout << "  Всего добавлений: " << totalCount << std::endl;
    
    auto data = generateWithDuplicates(uniqueCount, totalCount);
    
    std::unordered_set<uint64_t> exactSet;
    for (uint64_t id : data) {
        exactSet.insert(id);
    }
    size_t exactCount = exactSet.size();
    size_t setMemory = exactSet.bucket_count() * sizeof(void*) + exactSet.size() * sizeof(uint64_t);
    
    auto stdHasher = std::make_shared<StdHasher>();
    auto murmurHasher = std::make_shared<MurmurHasher>();
    
    std::cout << "\nРезультаты:" << std::endl;
    std::cout << "| p  | Hasher      | Оценка      | Ошибка (факт) | Ошибка (σ)  | Память    |" << std::endl;
    std::cout << "|----|-------------|-------------|---------------|-------------|-----------|" << std::endl;
    
    for (int p : {10, 12, 14}) {
        double expectedError = 1.04 / std::sqrt(1 << p) * 100;  // σ в процентах
        
        for (auto& [name, hasher] : std::vector<std::pair<std::string, HasherPtr>>{
            {"std::hash", stdHasher}, {"MurmurHash3", murmurHasher}}) {
            
            HyperLogLog hll(p, hasher);
            for (uint64_t id : data) {
                hll.add(id);
            }
            
            uint64_t estimate = hll.estimate();
            double error = std::abs(static_cast<double>(estimate) - exactCount) / exactCount * 100;
            
            std::cout << "| " << std::setw(2) << p
                      << " | " << std::setw(11) << name
                      << " | " << std::setw(11) << estimate
                      << " | " << std::setw(12) << std::fixed << std::setprecision(2) << error << "%"
                      << " | " << std::setw(10) << expectedError << "%"
                      << " | " << std::setw(9) << hll.getMemoryUsage()
                      << " |" << std::endl;
        }
    }
    
    std::cout << "| -- | exact       | " << std::setw(11) << exactCount
              << " | " << std::setw(12) << "0.00" << "%"
              << " | " << std::setw(10) << "-" << " "
              << " | " << std::setw(9) << setMemory
              << " |" << std::endl;
    
    double memorySaving = static_cast<double>(setMemory) / (16384 + sizeof(HyperLogLog));
    std::cout << "\nЭкономия памяти (p=14): " << std::fixed << std::setprecision(0) << memorySaving << "x" << std::endl;
    std::cout << "Ожидание: σ = 1.04 / sqrt(m), где m = 2^p" << std::endl;
}

// ============================================
// Бенчмарк Count-Min Sketch
// ============================================

void benchmarkCountMinSketch() {
    printHeader("Count-Min Sketch vs std::unordered_map");
    
    const size_t uniqueCount = 10000;
    const size_t totalCount = 1000000;
    
    std::cout << "\nПараметры:" << std::endl;
    std::cout << "  Уникальных элементов: " << uniqueCount << std::endl;
    std::cout << "  Всего добавлений (N): " << totalCount << std::endl;
    std::cout << "  Распределение: Zipf (экспонента = 1.0)" << std::endl;
    
    auto data = generateZipfDistribution(uniqueCount, totalCount);
    
    std::unordered_map<uint64_t, uint64_t> exactMap;
    for (uint64_t id : data) {
        exactMap[id]++;
    }
    size_t mapMemory = exactMap.bucket_count() * sizeof(void*) + 
                       exactMap.size() * (sizeof(uint64_t) * 2);
    
    auto stdHasher = std::make_shared<StdHasher>();
    auto murmurHasher = std::make_shared<MurmurHasher>();
    
    std::cout << "\nРезультаты:" << std::endl;
    std::cout << "| ε       | Hasher      | Ср.ошибка | Макс.ошибка | Макс.допуст.(ε×N) | Память    |" << std::endl;
    std::cout << "|---------|-------------|-----------|-------------|-------------------|-----------|" << std::endl;
    
    for (double eps : {0.01, 0.001}) {
        double maxAllowedError = eps * totalCount;
        
        for (auto& [name, hasher] : std::vector<std::pair<std::string, HasherPtr>>{
            {"std::hash", stdHasher}, {"MurmurHash3", murmurHasher}}) {
            
            CountMinSketch cms(eps, 0.01, hasher);
            for (uint64_t id : data) {
                cms.add(id);
            }
            
            double totalError = 0.0;
            double maxError = 0.0;
            for (const auto& [elem, count] : exactMap) {
                uint64_t estimate = cms.estimate(elem);
                double error = static_cast<double>(estimate) - count;
                totalError += error;
                maxError = std::max(maxError, error);
            }
            double avgError = totalError / exactMap.size();
            
            std::cout << "| " << std::setw(7) << std::fixed << std::setprecision(3) << eps
                      << " | " << std::setw(11) << name
                      << " | " << std::setw(9) << std::fixed << std::setprecision(2) << avgError
                      << " | " << std::setw(11) << std::fixed << std::setprecision(0) << maxError
                      << " | " << std::setw(17) << std::fixed << std::setprecision(0) << maxAllowedError
                      << " | " << std::setw(9) << cms.getMemoryUsage()
                      << " |" << std::endl;
        }
    }
    
    std::cout << "| exact   | -           | " << std::setw(9) << "0.00"
              << " | " << std::setw(11) << "0"
              << " | " << std::setw(17) << "-"
              << " | " << std::setw(9) << mapMemory
              << " |" << std::endl;
    
    // Экономия памяти для ε=0.001
    CountMinSketch cmsRef(0.001, 0.01);
    double memorySaving = static_cast<double>(mapMemory) / cmsRef.getMemoryUsage();
    std::cout << "\nЭкономия памяти (ε=0.001): " << std::fixed << std::setprecision(1) << memorySaving << "x" << std::endl;
    std::cout << "Гарантия: estimate(x) >= actual(x), ошибка <= ε×N с вероятностью >= 1-δ" << std::endl;
}

// ============================================
// Демонстрация гарантии CMS
// ============================================

void demonstrateNeverUnderestimates() {
    printHeader("Демонстрация: CMS никогда не занижает");
    
    std::cout << "\nДобавляем: video_A — 100 раз, video_B — 50 раз, video_C — 0 раз" << std::endl;
    
    CountMinSketch cms(0.1, 0.1);
    
    for (int i = 0; i < 100; ++i) cms.add(std::string("video_A"));
    for (int i = 0; i < 50; ++i) cms.add(std::string("video_B"));
    
    uint64_t estA = cms.estimate(std::string("video_A"));
    uint64_t estB = cms.estimate(std::string("video_B"));
    uint64_t estC = cms.estimate(std::string("video_C"));
    
    std::cout << "\n| Видео   | Реально | Оценка CMS | estimate >= actual? |" << std::endl;
    std::cout << "|---------|---------|------------|---------------------|" << std::endl;
    std::cout << "| video_A | 100     | " << std::setw(10) << estA 
              << " | " << (estA >= 100 ? "ДА ✓" : "НЕТ ✗") << std::setw(17) << " |" << std::endl;
    std::cout << "| video_B | 50      | " << std::setw(10) << estB 
              << " | " << (estB >= 50 ? "ДА ✓" : "НЕТ ✗") << std::setw(17) << " |" << std::endl;
    std::cout << "| video_C | 0       | " << std::setw(10) << estC 
              << " | " << (estC >= 0 ? "ДА ✓" : "НЕТ ✗") << std::setw(17) << " |" << std::endl;
}

// ============================================
// Main
// ============================================

int main() {
    std::cout << "============================================" << std::endl;
    std::cout << "  ДЗ-22: Вероятностные структуры данных" << std::endl;
    std::cout << "============================================" << std::endl;
    
    benchmarkBloomFilter();
    benchmarkHyperLogLog();
    benchmarkCountMinSketch();
    demonstrateNeverUnderestimates();
    
    std::cout << "\n";
    return 0;
}
