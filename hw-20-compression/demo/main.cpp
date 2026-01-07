#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <memory>
#include <random>
#include <chrono>
#include <string>
#include <cstring>
#include <algorithm>

#include "compression/ICompressor.hpp"
#include "compression/RLECompressor.hpp"
#include "compression/PackBitsCompressor.hpp"

// ============================================
// Генераторы тестовых данных
// ============================================

/**
 * @brief Генератор данных с повторами (идеально для RLE).
 * 
 * Имитирует: чёрно-белое изображение, иконки
 */
std::vector<uint8_t> generateRepeatingData(size_t size, unsigned int seed = 42) {
    std::vector<uint8_t> data;
    data.reserve(size);
    
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> byteDist(0, 255);
    std::uniform_int_distribution<int> runDist(10, 100);  // длинные серии
    
    while (data.size() < size) {
        uint8_t byte = static_cast<uint8_t>(byteDist(rng));
        size_t runLen = std::min(static_cast<size_t>(runDist(rng)), size - data.size());
        
        for (size_t i = 0; i < runLen; ++i) {
            data.push_back(byte);
        }
    }
    
    return data;
}

/**
 * @brief Генератор текстовых данных (средне для RLE).
 * 
 * Имитирует: текстовые файлы, исходный код
 */
std::vector<uint8_t> generateTextData(size_t size, unsigned int seed = 42) {
    const std::string loremIpsum = 
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
        "Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. "
        "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris. "
        "Duis aute irure dolor in reprehenderit in voluptate velit esse cillum. "
        "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia. ";
    
    std::vector<uint8_t> data;
    data.reserve(size);
    
    while (data.size() < size) {
        for (char c : loremIpsum) {
            if (data.size() >= size) break;
            data.push_back(static_cast<uint8_t>(c));
        }
    }
    
    return data;
}

/**
 * @brief Генератор случайных данных (плохо для RLE).
 * 
 * Имитирует: фотографии (JPEG внутри), аудио, видео, зашифрованные данные
 */
std::vector<uint8_t> generateRandomData(size_t size, unsigned int seed = 42) {
    std::vector<uint8_t> data(size);
    
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(0, 255);
    
    for (auto& byte : data) {
        byte = static_cast<uint8_t>(dist(rng));
    }
    
    return data;
}

/**
 * @brief Генератор "сжатых" данных (ужасно для RLE).
 * 
 * Имитирует: ZIP-архивы, уже сжатые файлы
 * Используем высокую энтропию без повторов
 */
std::vector<uint8_t> generateCompressedLikeData(size_t size, unsigned int seed = 42) {
    // Сжатые данные имеют высокую энтропию и почти нет повторов
    // Генерируем данные с равномерным распределением
    return generateRandomData(size, seed);
}

/**
 * @brief Генератор паттерна (хорошо для RLE).
 * 
 * Имитирует: градиенты, простые графические файлы
 */
std::vector<uint8_t> generatePatternData(size_t size, unsigned int seed = 42) {
    std::vector<uint8_t> data;
    data.reserve(size);
    
    // Паттерн: 0,0,0,1,1,1,2,2,2,...,255,255,255,0,0,0,...
    uint8_t value = 0;
    size_t runLen = 3;
    
    while (data.size() < size) {
        for (size_t i = 0; i < runLen && data.size() < size; ++i) {
            data.push_back(value);
        }
        ++value;  // Автоматический переход 255 → 0
    }
    
    return data;
}

// ============================================
// Структуры для результатов
// ============================================

struct BenchmarkResult {
    std::string algorithmName;
    std::string dataType;
    size_t originalSize;
    size_t compressedSize;
    double compressionRatio;
    double compressionPercent;
    double compressTimeUs;
    double decompressTimeUs;
    bool verified;
};

// ============================================
// Бенчмарк
// ============================================

BenchmarkResult runBenchmark(ICompressor& compressor,
                              const std::vector<uint8_t>& data,
                              const std::string& dataType,
                              int iterations) {
    BenchmarkResult result;
    result.algorithmName = compressor.getName();
    result.dataType = dataType;
    result.originalSize = data.size();
    
    // Сжатие
    auto startCompress = std::chrono::high_resolution_clock::now();
    std::vector<uint8_t> compressed;
    for (int i = 0; i < iterations; ++i) {
        compressed = compressor.compress(data);
    }
    auto endCompress = std::chrono::high_resolution_clock::now();
    
    result.compressTimeUs = std::chrono::duration_cast<std::chrono::microseconds>(
        endCompress - startCompress).count() / static_cast<double>(iterations);
    
    result.compressedSize = compressed.size();
    result.compressionRatio = ICompressor::compressionRatio(data.size(), compressed.size());
    result.compressionPercent = ICompressor::compressionPercent(data.size(), compressed.size());
    
    // Распаковка
    auto startDecompress = std::chrono::high_resolution_clock::now();
    std::vector<uint8_t> decompressed;
    for (int i = 0; i < iterations; ++i) {
        decompressed = compressor.decompress(compressed);
    }
    auto endDecompress = std::chrono::high_resolution_clock::now();
    
    result.decompressTimeUs = std::chrono::duration_cast<std::chrono::microseconds>(
        endDecompress - startDecompress).count() / static_cast<double>(iterations);
    
    // Проверка корректности
    result.verified = (data == decompressed);
    
    return result;
}

// ============================================
// Вывод результатов
// ============================================

void printUsage(const char* programName) {
    std::cout << "Использование:\n";
    std::cout << "  " << programName << " compress <algorithm> <input> <output>\n";
    std::cout << "  " << programName << " decompress <algorithm> <input> <output>\n";
    std::cout << "  " << programName << " benchmark\n";
    std::cout << "\n";
    std::cout << "Алгоритмы:\n";
    std::cout << "  rle      - базовый Run-Length Encoding\n";
    std::cout << "  packbits - улучшенный RLE (PackBits)\n";
    std::cout << "\n";
    std::cout << "Примеры:\n";
    std::cout << "  " << programName << " compress rle input.txt output.rle\n";
    std::cout << "  " << programName << " decompress rle output.rle restored.txt\n";
    std::cout << "  " << programName << " benchmark\n";
}

void printBenchmarkHeader() {
    std::cout << "\n";
    std::cout << "|  Алгоритм  |       Тип данных     |  Исходный  |   Сжатый   |  Коэфф.  |  Сжатие% | Сжат.(мкс) | Расп.(мкс) | OK |" << std::endl;

    std::cout << "|" << std::string(12, '-')
              << "|" << std::string(22, '-')
              << "|" << std::string(12, '-')
              << "|" << std::string(12, '-')
              << "|" << std::string(10, '-')
              << "|" << std::string(10, '-')
              << "|" << std::string(12, '-')
              << "|" << std::string(12, '-')
              << "|" << std::string(4, '-')
              << "|" << std::endl;
}

void printBenchmarkResult(const BenchmarkResult& r) {
    std::cout << "| " << std::left << std::setw(10) << r.algorithmName
              << " | " << std::setw(20) << r.dataType
              << " | " << std::right << std::setw(10) << r.originalSize
              << " | " << std::setw(10) << r.compressedSize
              << " | " << std::setw(8) << std::fixed << std::setprecision(2) << r.compressionRatio
              << " | " << std::setw(7) << std::fixed << std::setprecision(1) << r.compressionPercent << "%"
              << " | " << std::setw(10) << std::fixed << std::setprecision(1) << r.compressTimeUs
              << " | " << std::setw(10) << std::fixed << std::setprecision(1) << r.decompressTimeUs
              << " | " << std::setw(4) << (r.verified ? "✓" : "✗")
              << " |" << std::endl;
}

void printBenchmarkSeparator() {
    std::cout << "|" << std::string(12, '-')
              << "|" << std::string(22, '-')
              << "|" << std::string(12, '-')
              << "|" << std::string(12, '-')
              << "|" << std::string(10, '-')
              << "|" << std::string(10, '-')
              << "|" << std::string(12, '-')
              << "|" << std::string(12, '-')
              << "|" << std::string(4, '-')
              << "|" << std::endl;
}

// ============================================
// Работа с файлами
// ============================================

std::vector<uint8_t> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::vector<uint8_t> data(size);
    file.read(reinterpret_cast<char*>(data.data()), size);
    
    return data;
}

void writeFile(const std::string& filename, const std::vector<uint8_t>& data) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot create file: " + filename);
    }
    
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

std::unique_ptr<ICompressor> createCompressor(const std::string& name) {
    if (name == "rle") {
        return std::make_unique<RLECompressor>();
    } else if (name == "packbits") {
        return std::make_unique<PackBitsCompressor>();
    }
    return nullptr;
}

// ============================================
// Main
// ============================================

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }
    
    std::string command = argv[1];
    
    if (command == "benchmark") {
        // ==========================================
        // Режим бенчмарка
        // ==========================================
        
        std::cout << "============================================" << std::endl;
        std::cout << "  ДЗ-20: Алгоритмы сжатия" << std::endl;
        std::cout << "  Бенчмарки RLE и PackBits" << std::endl;
        std::cout << "============================================" << std::endl;
        
        std::cout << "\nГенерация тестовых данных..." << std::endl;
        std::cout << "(Имитация разных типов файлов через генерацию)\n" << std::endl;
        
        const size_t dataSize = 100000;  // 100 KB
        const int iterations = 100;
        
        // Генерируем тестовые данные
        struct TestData {
            std::string name;
            std::vector<uint8_t> data;
        };
        
        std::vector<TestData> testDatasets = {
            {
                "Repeating (icon)",
                // Имитирует: иконки, чёрно-белые изображения с большими однотонными областями
                generateRepeatingData(dataSize)
            },
            {
                "Pattern (gradient)",
                // Имитирует: градиенты, простые графические файлы
                generatePatternData(dataSize)
            },
            {
                "Text (Lorem)",
                // Имитирует: текстовые файлы, исходный код
                generateTextData(dataSize)
            },
            {
                "Random (photo/audio)",
                // Имитирует: фотографии, аудио, видео (высокая энтропия)
                generateRandomData(dataSize)
            },
            {
                "Compressed (ZIP)",
                // Имитирует: уже сжатые данные (ZIP, JPEG и т.д.)
                generateCompressedLikeData(dataSize, 123)
            }
        };
        
        // Создаём компрессоры
        RLECompressor rle;
        PackBitsCompressor packbits;
        
        printBenchmarkHeader();
        
        for (const auto& testData : testDatasets) {
            auto rleResult = runBenchmark(rle, testData.data, testData.name, iterations);
            auto packbitsResult = runBenchmark(packbits, testData.data, testData.name, iterations);
            
            printBenchmarkResult(rleResult);
            printBenchmarkResult(packbitsResult);
            printBenchmarkSeparator();
        }
        
        // Выводы
        std::cout << "\n";
        std::cout << "============================================" << std::endl;
        std::cout << "  Описание тестовых данных" << std::endl;
        std::cout << "============================================" << std::endl;
        std::cout << "\n";
        std::cout << "1. Repeating (icon) — длинные серии одинаковых байт\n";
        std::cout << "   Имитирует: иконки, чёрно-белые изображения\n";
        std::cout << "\n";
        std::cout << "2. Pattern (gradient) — повторяющийся паттерн 0,0,0,1,1,1,...\n";
        std::cout << "   Имитирует: градиенты, простые графические файлы\n";
        std::cout << "\n";
        std::cout << "3. Text (Lorem) — текст Lorem Ipsum\n";
        std::cout << "   Имитирует: текстовые файлы, исходный код\n";
        std::cout << "\n";
        std::cout << "4. Random (photo/audio) — случайные байты\n";
        std::cout << "   Имитирует: фотографии, аудио, видео (высокая энтропия)\n";
        std::cout << "\n";
        std::cout << "5. Compressed (ZIP) — случайные данные с высокой энтропией\n";
        std::cout << "   Имитирует: ZIP-архивы, уже сжатые файлы\n";
        std::cout << "\n";
        std::cout << "============================================" << std::endl;
        std::cout << "  Выводы" << std::endl;
        std::cout << "============================================" << std::endl;
        std::cout << "\n";
        std::cout << "1. RLE эффективен только для данных с длинными сериями повторов.\n";
        std::cout << "   На случайных данных размер УДВАИВАЕТСЯ.\n";
        std::cout << "\n";
        std::cout << "2. PackBits решает проблему раздувания:\n";
        std::cout << "   - На повторах работает как RLE\n";
        std::cout << "   - На случайных данных overhead < 1%\n";
        std::cout << "\n";
        std::cout << "3. Рекомендации:\n";
        std::cout << "   - Для простых изображений: RLE или PackBits\n";
        std::cout << "   - Для универсального сжатия: DEFLATE (zlib)\n";
        std::cout << "   - Для максимального сжатия: LZMA (7-zip)\n";
        std::cout << "\n";
        
        return 0;
    }
    else if (command == "compress" || command == "decompress") {
        // ==========================================
        // Режим сжатия/распаковки файлов
        // ==========================================
        
        if (argc != 5) {
            printUsage(argv[0]);
            return 1;
        }
        
        std::string algorithm = argv[2];
        std::string inputFile = argv[3];
        std::string outputFile = argv[4];
        
        auto compressor = createCompressor(algorithm);
        if (!compressor) {
            std::cerr << "Неизвестный алгоритм: " << algorithm << std::endl;
            return 1;
        }
        
        try {
            auto inputData = readFile(inputFile);
            std::vector<uint8_t> outputData;
            
            if (command == "compress") {
                outputData = compressor->compress(inputData);
                std::cout << "Сжатие: " << inputFile << " → " << outputFile << std::endl;
                std::cout << "Алгоритм: " << compressor->getName() << std::endl;
                std::cout << "Исходный размер: " << inputData.size() << " байт" << std::endl;
                std::cout << "Сжатый размер: " << outputData.size() << " байт" << std::endl;
                std::cout << "Коэффициент сжатия: " << std::fixed << std::setprecision(2)
                          << ICompressor::compressionRatio(inputData.size(), outputData.size()) << std::endl;
                std::cout << "Степень сжатия: " << std::fixed << std::setprecision(1)
                          << ICompressor::compressionPercent(inputData.size(), outputData.size()) << "%" << std::endl;
            } else {
                outputData = compressor->decompress(inputData);
                std::cout << "Распаковка: " << inputFile << " → " << outputFile << std::endl;
                std::cout << "Алгоритм: " << compressor->getName() << std::endl;
                std::cout << "Сжатый размер: " << inputData.size() << " байт" << std::endl;
                std::cout << "Распакованный размер: " << outputData.size() << " байт" << std::endl;
            }
            
            writeFile(outputFile, outputData);
            std::cout << "Готово!" << std::endl;
            
        } catch (const std::exception& e) {
            std::cerr << "Ошибка: " << e.what() << std::endl;
            return 1;
        }
        
        return 0;
    }
    else {
        printUsage(argv[0]);
        return 1;
    }
}
