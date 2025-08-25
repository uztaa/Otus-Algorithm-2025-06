#include "FileGenerator.h"
#include "RandomArrayGenerator.h"

FileGenerator::FileGenerator(std::shared_ptr<FileService> fileService) : fs_(std::move(fileService)) {}

bool FileGenerator::generateFile(const std::string& filename, size_t rows, int maxValue, uint32_t seed) {
    RandomArrayGenerator gen(seed, 1, maxValue);
    std::vector<Record> records = gen.generate(rows);

    std::vector<int> numbers;
    numbers.reserve(rows);
    for (const auto& r : records) {
        numbers.push_back(r.getKey());
    }

    return fs_->writeLines(filename, numbers);
}
