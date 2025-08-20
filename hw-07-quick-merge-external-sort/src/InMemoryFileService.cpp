#include "InMemoryFileService.h"

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
