#pragma once

#include "FileService.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <mutex>

/**
 * @brief Реализация FileService, хранящая файлы в памяти.
 *
 * Использует потокобезопасный map для хранения данных.
 * Подходит для юнит-тестов и сценариев с ограниченной I/O.
 */
class InMemoryFileService : public FileService
{
public:
    InMemoryFileService() = default;
    virtual ~InMemoryFileService();

    bool createFile(const std::string &path) override;
    bool writeLines(const std::string &path, const std::vector<int> &lines) override;
    bool appendLines(const std::string &path, const std::vector<int> &lines) override;
    bool readLines(const std::string &path, std::vector<int> &outLines) override;
    bool deleteFile(const std::string &path) override;
    bool fileExists(const std::string &path) override;
    bool splitFileToChunks(const std::string &inputFile, int chunks, int maxKey, std::vector<std::string> &chunkFiles) override;
    bool mergeChunks(const std::vector<std::string> &chunkFiles, const std::string &outputFile) override;

private:
    std::unordered_map<std::string, std::vector<int>> files_;
    std::mutex mtx_;
};
