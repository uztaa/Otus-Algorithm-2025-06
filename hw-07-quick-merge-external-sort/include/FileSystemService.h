#pragma once

#include "FileService.h"
#include <string>
#include <vector>
#include "FileSystemSettings.h"

/**
 * @brief Конкретная реализация FileService для работы с файловой системой.
 * Используется стандартная библиотека std::fstream и std::filesystem.
 */
class FileSystemService : public FileService
{
private:
    // директория, относительно которой будут создаваться файлы
    std::string rootDir;
    // получить реальный путь в системе
    const std::string getRealPath(const std::string &path) const;

public:
    // Создаем папку (со всей иерархией) для работы конкретного сервиса.
    // Разные объекты-сервисы работают в своих подпапках: удобно для распараллеливания.
    explicit FileSystemService(const std::string &root = ROOT_DIR);
    
    // деструктор - удаляет папку, в которой работал сервис файловой системы
    virtual ~FileSystemService();
    
    // для внешнего обращения - относительный путь, 
    // внутри сервис работает с реальным путем,
    // учитывая rootDir
    bool createFile(const std::string &path) override;
    bool writeLines(const std::string &path, const std::vector<int> &lines) override;
    bool appendLines(const std::string &path, const std::vector<int> &lines) override;
    bool readLines(const std::string &path, std::vector<int> &outLines) override;
    bool deleteFile(const std::string &path) override;
    bool fileExists(const std::string &path) override;
    bool splitFileToChunks(const std::string &inputFile, int chunks, int maxKey, std::vector<std::string> &chunkFiles) override;
    bool mergeChunks(const std::vector<std::string> &chunkFiles, const std::string &outputFile) override;
};
