#include "FileGenerator.h"
#include "RandomArrayGenerator.h"

FileGenerator::FileGenerator(std::shared_ptr<FileService> fileService) : fs_(std::move(fileService)) {}

bool FileGenerator::generateFile(const std::string& filename, size_t N, int T, uint32_t seed) {
    RandomArrayGenerator gen;
    std::vector<Record> records = gen.regenerate(N, seed);

    std::vector<int> numbers;
    numbers.reserve(N);
    for (const auto& r : records) {
        int val = r.getKey();
        if (val < 1) val = 1;
        else if (val > T) val = T;
        numbers.push_back(val);
    }

    return fs_->writeLines(filename, numbers);
}
