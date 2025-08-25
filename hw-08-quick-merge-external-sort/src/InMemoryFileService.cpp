#include "InMemoryFileService.h"

InMemoryFileService::~InMemoryFileService() {
    files_.clear();
}

bool InMemoryFileService::createFile(const std::string& path) {
    std::lock_guard<std::mutex> lock(mtx_);
    files_[path] = {}; // Очищаем или создаем пустой файл в памяти
    return true;
}

bool InMemoryFileService::writeLines(const std::string& path, const std::vector<int>& lines) {
    std::lock_guard<std::mutex> lock(mtx_);
    files_[path] = lines;
    return true;
}

bool InMemoryFileService::appendLines(const std::string& path, const std::vector<int>& lines) {
    std::lock_guard<std::mutex> lock(mtx_);
    auto &fileData = files_[path];
    fileData.insert(fileData.end(), lines.begin(), lines.end());
    return true;
}

bool InMemoryFileService::readLines(const std::string& path, std::vector<int>& outLines) {
    std::lock_guard<std::mutex> lock(mtx_);
    auto it = files_.find(path);
    if (it == files_.end()) return false;
    outLines = it->second;
    return true;
}

bool InMemoryFileService::deleteFile(const std::string& path) {
    std::lock_guard<std::mutex> lock(mtx_);
    return files_.erase(path) > 0;
}

bool InMemoryFileService::fileExists(const std::string& path) {
    std::lock_guard<std::mutex> lock(mtx_);
    return files_.find(path) != files_.end();
}

bool InMemoryFileService::splitFileToChunks(const std::string& inputFile, int T, int maxKey, std::vector<std::string>& chunkFiles) {
    std::lock_guard<std::mutex> lock(mtx_);
    chunkFiles.clear();

    auto it = files_.find(inputFile);
    if (it == files_.end()) return false;

    // Инициализируем T пустых векторов для chunk-ов
    std::vector<std::vector<int>> chunks(T);

    int interval = (maxKey + T - 1) / T;

    for (const int& val : it->second) {
        int idx = val / interval;
        if (idx >= T) idx = T - 1;
        chunks[idx].push_back(val);
    }

    // Сохраняем chunk-ы как новые файлы в памяти
    for (int i = 0; i < T; ++i) {
        std::string chunkName = "chunk_" + std::to_string(i) + ".txt";
        files_[chunkName] = std::move(chunks[i]);
        chunkFiles.push_back(chunkName);
    }

    return true;
}

bool InMemoryFileService::mergeChunks(const std::vector<std::string>& chunkFiles, const std::string& outputFile) {
    std::lock_guard<std::mutex> lock(mtx_);
    std::vector<int> merged;

    for (const auto& file : chunkFiles) {
        auto it = files_.find(file);
        if (it == files_.end()) return false;

        merged.insert(merged.end(), it->second.begin(), it->second.end());
    }
    files_[outputFile] = std::move(merged);
    return true;
}
