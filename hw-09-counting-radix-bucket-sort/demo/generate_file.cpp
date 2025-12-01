#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <chrono>
#include <cstdint>

/**
 * @brief Генератор бинарного файла с 16-битными числами
 * 
 * Использование: ./generate_file <count> [output_file] [seed]
 * 
 * По умолчанию:
 *   output_file = "data.bin"
 *   seed = 12345
 */
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <count> [output_file] [seed]\n";
        std::cerr << "  count       - number of 16-bit integers to generate\n";
        std::cerr << "  output_file - output file name (default: data.bin)\n";
        std::cerr << "  seed        - random seed (default: 12345)\n";
        std::cerr << "\nExamples:\n";
        std::cerr << "  " << argv[0] << " 1000000000           # 1 billion numbers (~1.9 GB)\n";
        std::cerr << "  " << argv[0] << " 100000000 test.bin   # 100 million numbers (~190 MB)\n";
        return 1;
    }

    size_t count = std::stoull(argv[1]);
    std::string outputFile = (argc > 2) ? argv[2] : "data.bin";
    uint32_t seed = (argc > 3) ? std::stoul(argv[3]) : 12345u;

    std::cout << "Generating " << count << " 16-bit numbers to '" << outputFile << "'...\n";
    std::cout << "Expected file size: " << (count * 2 / 1024.0 / 1024.0) << " MB\n";

    auto startTime = std::chrono::steady_clock::now();

    std::ofstream file(outputFile, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Cannot open file for writing: " << outputFile << "\n";
        return 1;
    }

    std::mt19937 rng(seed);
    std::uniform_int_distribution<uint16_t> dist(0, 65535);

    // Буферизованная запись для производительности
    constexpr size_t BUFFER_SIZE = 1024 * 1024; // 1 MB buffer = 512K numbers
    std::vector<uint16_t> buffer;
    buffer.reserve(BUFFER_SIZE / sizeof(uint16_t));

    size_t written = 0;
    size_t lastProgress = 0;

    while (written < count) {
        buffer.clear();
        size_t batchSize = std::min(BUFFER_SIZE / sizeof(uint16_t), count - written);
        
        for (size_t i = 0; i < batchSize; ++i) {
            buffer.push_back(dist(rng));
        }

        file.write(reinterpret_cast<const char*>(buffer.data()), 
                   buffer.size() * sizeof(uint16_t));
        
        written += batchSize;

        // Прогресс каждые 10%
        size_t progress = (written * 100) / count;
        if (progress >= lastProgress + 10) {
            std::cout << "  Progress: " << progress << "%\n";
            lastProgress = progress;
        }
    }

    file.close();

    auto endTime = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

    std::cout << "Done! Generated " << written << " numbers in " 
              << (duration.count() / 1000.0) << " seconds.\n";

    return 0;
}