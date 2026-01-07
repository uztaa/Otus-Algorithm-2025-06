#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <vector>
#include <random>
#include <numeric>

#include "compression/ICompressor.hpp"
#include "compression/RLECompressor.hpp"
#include "compression/PackBitsCompressor.hpp"

// ============================================
// Параметризованные тесты для всех алгоритмов
// ============================================

enum class AlgorithmType {
    RLE,
    PackBits
};

std::string algorithmName(AlgorithmType type) {
    switch (type) {
        case AlgorithmType::RLE: return "RLE";
        case AlgorithmType::PackBits: return "PackBits";
    }
    return "Unknown";
}

class CompressionTest : public ::testing::TestWithParam<AlgorithmType> {
protected:
    std::unique_ptr<ICompressor> createCompressor() {
        switch (GetParam()) {
            case AlgorithmType::RLE:
                return std::make_unique<RLECompressor>();
            case AlgorithmType::PackBits:
                return std::make_unique<PackBitsCompressor>();
        }
        return nullptr;
    }
};

// ============================================
// Базовые тесты
// ============================================

TEST_P(CompressionTest, EmptyData) {
    auto compressor = createCompressor();
    
    std::vector<uint8_t> empty;
    auto compressed = compressor->compress(empty);
    auto decompressed = compressor->decompress(compressed);
    
    EXPECT_TRUE(decompressed.empty());
}

TEST_P(CompressionTest, SingleByte) {
    auto compressor = createCompressor();
    
    std::vector<uint8_t> data = {0x42};
    auto compressed = compressor->compress(data);
    auto decompressed = compressor->decompress(compressed);
    
    EXPECT_EQ(data, decompressed);
}

TEST_P(CompressionTest, TwoSameBytes) {
    auto compressor = createCompressor();
    
    std::vector<uint8_t> data = {0x41, 0x41};
    auto compressed = compressor->compress(data);
    auto decompressed = compressor->decompress(compressed);
    
    EXPECT_EQ(data, decompressed);
}

TEST_P(CompressionTest, TwoDifferentBytes) {
    auto compressor = createCompressor();
    
    std::vector<uint8_t> data = {0x41, 0x42};
    auto compressed = compressor->compress(data);
    auto decompressed = compressor->decompress(compressed);
    
    EXPECT_EQ(data, decompressed);
}

TEST_P(CompressionTest, LongRunOfSameBytes) {
    auto compressor = createCompressor();
    
    // 100 одинаковых байт
    std::vector<uint8_t> data(100, 0xAA);
    auto compressed = compressor->compress(data);
    auto decompressed = compressor->decompress(compressed);
    
    EXPECT_EQ(data, decompressed);
    // Сжатие должно быть эффективным
    EXPECT_LT(compressed.size(), data.size());
}

TEST_P(CompressionTest, MaxRunLength255) {
    auto compressor = createCompressor();
    
    // 255 одинаковых байт
    std::vector<uint8_t> data(255, 0xBB);
    auto compressed = compressor->compress(data);
    auto decompressed = compressor->decompress(compressed);
    
    EXPECT_EQ(data, decompressed);
}

TEST_P(CompressionTest, LongerThan255Run) {
    auto compressor = createCompressor();
    
    // 300 одинаковых байт — должно разбиться на несколько блоков
    std::vector<uint8_t> data(300, 0xCC);
    auto compressed = compressor->compress(data);
    auto decompressed = compressor->decompress(compressed);
    
    EXPECT_EQ(data, decompressed);
}

TEST_P(CompressionTest, AlternatingBytes) {
    auto compressor = createCompressor();
    
    // ABABAB...
    std::vector<uint8_t> data;
    for (int i = 0; i < 100; ++i) {
        data.push_back(i % 2 == 0 ? 0x41 : 0x42);
    }
    
    auto compressed = compressor->compress(data);
    auto decompressed = compressor->decompress(compressed);
    
    EXPECT_EQ(data, decompressed);
}

TEST_P(CompressionTest, MixedData) {
    auto compressor = createCompressor();
    
    // AAABBBCCCDDD + разные байты
    std::vector<uint8_t> data = {
        0x41, 0x41, 0x41,           // AAA
        0x42, 0x42, 0x42,           // BBB
        0x43, 0x44, 0x45, 0x46,     // CDEF
        0x47, 0x47, 0x47, 0x47, 0x47 // GGGGG
    };
    
    auto compressed = compressor->compress(data);
    auto decompressed = compressor->decompress(compressed);
    
    EXPECT_EQ(data, decompressed);
}

TEST_P(CompressionTest, AllByteValues) {
    auto compressor = createCompressor();
    
    // Все возможные значения байта 0-255
    std::vector<uint8_t> data(256);
    std::iota(data.begin(), data.end(), 0);
    
    auto compressed = compressor->compress(data);
    auto decompressed = compressor->decompress(compressed);
    
    EXPECT_EQ(data, decompressed);
}

TEST_P(CompressionTest, RandomData) {
    auto compressor = createCompressor();
    
    // Случайные данные
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(0, 255);
    
    std::vector<uint8_t> data(1000);
    for (auto& byte : data) {
        byte = static_cast<uint8_t>(dist(rng));
    }
    
    auto compressed = compressor->compress(data);
    auto decompressed = compressor->decompress(compressed);
    
    EXPECT_EQ(data, decompressed);
}

TEST_P(CompressionTest, GetNameNotEmpty) {
    auto compressor = createCompressor();
    EXPECT_FALSE(compressor->getName().empty());
}

// ============================================
// Параметризация
// ============================================

INSTANTIATE_TEST_SUITE_P(
    AllAlgorithms,
    CompressionTest,
    ::testing::Values(
        AlgorithmType::RLE,
        AlgorithmType::PackBits
    ),
    [](const testing::TestParamInfo<AlgorithmType>& info) {
        return algorithmName(info.param);
    }
);

// ============================================
// Специфичные тесты для RLE
// ============================================

TEST(RLECompressorTest, SimpleCompression) {
    RLECompressor rle;
    
    // AAA → 03 41
    std::vector<uint8_t> data = {0x41, 0x41, 0x41};
    auto compressed = rle.compress(data);
    
    ASSERT_EQ(compressed.size(), 2u);
    EXPECT_EQ(compressed[0], 3);   // count
    EXPECT_EQ(compressed[1], 0x41); // byte
}

TEST(RLECompressorTest, DataExpansion) {
    RLECompressor rle;
    
    // ABCD → 01 41 01 42 01 43 01 44 (раздувается в 2 раза)
    std::vector<uint8_t> data = {0x41, 0x42, 0x43, 0x44};
    auto compressed = rle.compress(data);
    
    EXPECT_EQ(compressed.size(), 8u);  // 4 * 2 = 8
}

// ============================================
// Специфичные тесты для PackBits
// ============================================

TEST(PackBitsCompressorTest, LiteralsOnly) {
    PackBitsCompressor packbits;
    
    // ABCD — литералы, не должно сильно раздуваться
    std::vector<uint8_t> data = {0x41, 0x42, 0x43, 0x44};
    auto compressed = packbits.compress(data);
    
    // Ожидаем: 03 41 42 43 44 (флаг=3 означает 4 литерала)
    EXPECT_LE(compressed.size(), data.size() + 2);  // +2 на флаг(и)
}

TEST(PackBitsCompressorTest, RunOnly) {
    PackBitsCompressor packbits;
    
    // AAAA — серия из 4
    std::vector<uint8_t> data = {0x41, 0x41, 0x41, 0x41};
    auto compressed = packbits.compress(data);
    
    // Ожидаем: FD 41 (флаг=-3 означает 4 повтора)
    ASSERT_EQ(compressed.size(), 2u);
    EXPECT_EQ(static_cast<int8_t>(compressed[0]), -3);  // 1 - 4 = -3
    EXPECT_EQ(compressed[1], 0x41);
}

TEST(PackBitsCompressorTest, MixedRunsAndLiterals) {
    PackBitsCompressor packbits;
    
    // AAABCD — серия AAA + литералы BCD
    std::vector<uint8_t> data = {0x41, 0x41, 0x41, 0x42, 0x43, 0x44};
    auto compressed = packbits.compress(data);
    auto decompressed = packbits.decompress(compressed);
    
    EXPECT_EQ(data, decompressed);
    // AAA как повтор: 2 байта
    // BCD как литералы: 4 байта
    // Итого ≈ 6 байт (не раздувается!)
    EXPECT_LE(compressed.size(), data.size() + 2);
}

TEST(PackBitsCompressorTest, NoBloatOnRandomData) {
    PackBitsCompressor packbits;
    
    // Случайные данные — PackBits не должен сильно раздувать
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(0, 255);
    
    std::vector<uint8_t> data(1000);
    for (auto& byte : data) {
        byte = static_cast<uint8_t>(dist(rng));
    }
    
    auto compressed = packbits.compress(data);
    
    // Worst case для PackBits: ~1% overhead (1 флаг на 128 байт)
    double ratio = static_cast<double>(compressed.size()) / data.size();
    EXPECT_LT(ratio, 1.1);  // Не более 10% раздувания
}

// ============================================
// Тесты утилитных функций
// ============================================

TEST(ICompressorTest, CompressionRatio) {
    EXPECT_DOUBLE_EQ(ICompressor::compressionRatio(100, 50), 2.0);
    EXPECT_DOUBLE_EQ(ICompressor::compressionRatio(100, 100), 1.0);
    EXPECT_DOUBLE_EQ(ICompressor::compressionRatio(100, 200), 0.5);
}

TEST(ICompressorTest, CompressionPercent) {
    EXPECT_DOUBLE_EQ(ICompressor::compressionPercent(100, 50), 50.0);
    EXPECT_DOUBLE_EQ(ICompressor::compressionPercent(100, 100), 0.0);
    EXPECT_DOUBLE_EQ(ICompressor::compressionPercent(100, 200), -100.0);
}

// ============================================
// Тесты на повреждённые данные
// ============================================

TEST(RLECompressorTest, CorruptedOddBytes) {
    RLECompressor rle;
    
    // Нечётное количество байт — невалидно
    std::vector<uint8_t> corrupted = {0x03, 0x41, 0x02};
    EXPECT_THROW(rle.decompress(corrupted), std::runtime_error);
}

TEST(RLECompressorTest, CorruptedZeroCount) {
    RLECompressor rle;
    
    // Нулевой count — невалидно
    std::vector<uint8_t> corrupted = {0x00, 0x41};
    EXPECT_THROW(rle.decompress(corrupted), std::runtime_error);
}

TEST(PackBitsCompressorTest, CorruptedTruncatedLiterals) {
    PackBitsCompressor packbits;
    
    // Флаг говорит "4 литерала", но данных только 2
    std::vector<uint8_t> corrupted = {0x03, 0x41, 0x42};  // flag=3 → 4 литерала
    EXPECT_THROW(packbits.decompress(corrupted), std::runtime_error);
}

TEST(PackBitsCompressorTest, CorruptedMissingRepeatByte) {
    PackBitsCompressor packbits;
    
    // Флаг повтора, но нет байта для повторения
    std::vector<uint8_t> corrupted = {0xFE};  // flag=-2 → 3 повтора, но байта нет
    EXPECT_THROW(packbits.decompress(corrupted), std::runtime_error);
}

TEST(PackBitsCompressorTest, CorruptedTruncatedRepeat) {
    PackBitsCompressor packbits;
    
    // Два блока, второй обрезан
    std::vector<uint8_t> corrupted = {0x00, 0x41, 0xFE};  // 1 литерал OK, потом повтор без байта
    EXPECT_THROW(packbits.decompress(corrupted), std::runtime_error);
}

// ============================================
// Тесты на декомпрессионную бомбу
// ============================================

TEST(RLECompressorTest, DecompressionBombProtection) {
    RLECompressor rle;
    
    // Создаём данные, которые распакуются в огромный размер
    // 255 повторов × много пар
    std::vector<uint8_t> bomb;
    for (int i = 0; i < 1000000; ++i) {  // 2MB сжатых → 255MB распакованных
        bomb.push_back(0xFF);  // count = 255
        bomb.push_back(0x00);  // byte = 0
    }
    
    EXPECT_THROW(rle.decompress(bomb), std::runtime_error);
}

TEST(PackBitsCompressorTest, DecompressionBombProtection) {
    PackBitsCompressor packbits;
    
    // Создаём данные с максимальными повторами
    std::vector<uint8_t> bomb;
    for (int i = 0; i < 1000000; ++i) {  // ~2MB сжатых → 128MB распакованных
        bomb.push_back(0x81);  // flag = -127 → 128 повторов
        bomb.push_back(0x00);  // byte = 0
    }
    
    EXPECT_THROW(packbits.decompress(bomb), std::runtime_error);
}
