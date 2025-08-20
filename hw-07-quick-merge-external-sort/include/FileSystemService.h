#pragma once

#include "FileService.h"
#include <string>
#include <vector>

/**
 * @brief Конкретная реализация FileService для работы с файловой системой.
 * Используется стандартная библиотека std::fstream и std::filesystem.
 */
class FileSystemService : public FileService
{
public:
    bool createFile(const std::string &path) override;
    bool writeLines(const std::string &path, const std::vector<int> &lines) override;
    bool appendLines(const std::string &path, const std::vector<int> &lines) override;
    bool readLines(const std::string &path, std::vector<int> &outLines) override;
    bool deleteFile(const std::string &path) override;
    bool fileExists(const std::string &path) override;
    bool splitFileToChunks(const std::string &inputFile, int chunks, int maxKey, std::vector<std::string> &chunkFiles) override;
    bool mergeChunks(const std::vector<std::string> &chunkFiles, const std::string &outputFile) override;
};
