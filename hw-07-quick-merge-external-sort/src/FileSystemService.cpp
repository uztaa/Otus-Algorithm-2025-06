#include "FileSystemService.h"
#include <fstream>
#include <filesystem>
#include <iostream>
#include <filesystem> // для std::filesystem::file_size
#include <stdexcept>
#include "RandomArrayGenerator.h"


FileSystemService::FileSystemService(const std::string &root)
{
    namespace fs = std::filesystem;

    // генерируем подпапку (примитивный UUID)
    RandomArrayGenerator rg;
    auto v = rg.generate(1);
    std::string uuid = std::to_string(v[0].getKey());

    // создаем полный путь до папки, где будет работать сервис
    rootDir = root + "/test" + uuid;

    std::error_code ec;
    if (!fs::exists(rootDir, ec))
    {
        // Папка не существует, создаём с рекурсивным созданием иерархии
        if (!fs::create_directories(rootDir, ec))
        {
            throw std::runtime_error("Failed to create root directory: " + rootDir + ". Reason: " + ec.message());
        }
    }
    else if (!fs::is_directory(rootDir, ec))
    {
        throw std::runtime_error("Path exists but is not a directory: " + rootDir);
    }
    if (ec)
    {
        throw std::runtime_error("Filesystem error for path " + rootDir + ": " + ec.message());
    }

    // выводим в консоль информацию о полном пути, если не было ошибок создания
    std::cout << "dir created: " << rootDir << std::endl;
};

FileSystemService::~FileSystemService() {
    std::error_code ec;
    if (!rootDir.empty()) {
        std::filesystem::remove_all(rootDir, ec);
        if (ec) {
            std::cerr << "Failed to remove directory " << rootDir 
                      << ": " << ec.message() << std::endl;
        }
    }
    std::cout << "dir deleted: " << rootDir << std::endl;
}

const std::string FileSystemService::getRealPath(const std::string &path) const
{
    return rootDir + "/" + path;
}

bool FileSystemService::createFile(const std::string &path)
{
    std::ofstream ofs(getRealPath(path), std::ios::trunc);
    return ofs.is_open();
}

bool FileSystemService::writeLines(const std::string &path, const std::vector<int> &lines)
{
    std::ofstream ofs(getRealPath(path), std::ios::trunc);
    if (!ofs.is_open())
        return false;
    for (int val : lines)
    {
        ofs << val << '\n';
    }
    return true;
}

bool FileSystemService::appendLines(const std::string &path, const std::vector<int> &lines)
{
    std::ofstream ofs(getRealPath(path), std::ios::app);
    if (!ofs.is_open())
        return false;
    for (int val : lines)
    {
        ofs << val << '\n';
    }
    return true;
}

bool FileSystemService::readLines(const std::string &path, std::vector<int> &outLines)
{
    std::ifstream ifs(getRealPath(path));
    if (!ifs.is_open())
        return false;
    outLines.clear();
    int val;
    while (ifs >> val)
    {
        outLines.push_back(val);
    }
    return true;
}

bool FileSystemService::deleteFile(const std::string &path)
{
    std::error_code ec;
    std::string realPath = getRealPath(path);
    bool removed = std::filesystem::remove(realPath, ec);
    if (!removed)
    {
        std::cerr << "Failed to delete file: " << realPath << ", error: " << ec.message() << std::endl;
    }
    return removed;
}

bool FileSystemService::fileExists(const std::string &path)
{
    return std::filesystem::exists(getRealPath(path));
}

bool FileSystemService::splitFileToChunks(const std::string &inputFile, int T, int maxKey, std::vector<std::string> &chunkFiles)
{
    chunkFiles.clear();

    // Создание пустых файлов перед записью
    for (int i = 0; i < T; ++i)
    {
        std::string filename = "chunk_" + std::to_string(i) + ".txt";
        if (!createFile(filename))
        {
            std::cerr << "Failed to create chunk file: " << getRealPath(filename) << std::endl;
            return false;
        }
        chunkFiles.push_back(filename);
    }

    std::string realInputFile = getRealPath(inputFile);
    std::ifstream ifs(realInputFile);
    if (!ifs.is_open())
    {
        std::cerr << "Failed to open input file: " << realInputFile << std::endl;
        return false;
    }

    // FIXME: тут логика адовая, надо разобраться - очень плохое разбиение
    int interval = (maxKey + T - 1) / T;
    int val;
    while (ifs >> val)
    {
        int idx = val / interval;
        if (idx >= T)
            idx = T - 1;

        // Открываем соответствующий файл чанка, записываем и закрываем сразу
        std::string chunkRealPath = getRealPath(chunkFiles[idx]);
        std::ofstream ofs(chunkRealPath, std::ios::app);
        if (!ofs.is_open())
        {
            std::cerr << "Failed to open chunk file for append: " << chunkRealPath << std::endl;
            return false;
        }
        ofs << val << '\n';
    }

    return true;
}

bool FileSystemService::mergeChunks(const std::vector<std::string> &chunkFiles, const std::string &outputFile)
{
    std::string realOut = getRealPath(outputFile);
    std::ofstream ofs(realOut, std::ios::trunc);
    if (!ofs.is_open())
    {
        std::cerr << "Failed to open output file: " << realOut << std::endl;
        return false;
    }

    for (const auto &file : chunkFiles)
    {
        std::string realFile = getRealPath(file);

        // Проверяем размер файла чанка
        std::error_code ec;
        auto fileSize = std::filesystem::file_size(realFile, ec);
        if (ec)
        {
            std::cerr << "Failed to get file size for chunk file: " << realFile
                      << ", error: " << ec.message() << std::endl;
            return false;
        }

        if (fileSize == 0)
        {
            // Пустой файл, пропускаем
            continue;
        }

        std::ifstream ifs(realFile);
        if (!ifs.is_open())
        {
            std::cerr << "Failed to open chunk file: " << realFile << std::endl;
            return false;
        }

        ofs << ifs.rdbuf();
        if (!ofs)
        {
            std::cerr << "Failed writing data from " << realFile << " to " << realOut << std::endl;
            return false;
        }
    }

    ofs.flush();
    if (!ofs)
    {
        std::cerr << "Failed to flush output file: " << realOut << std::endl;
        return false;
    }

    // Удаляем чанки
    for (const auto &chunkFile : chunkFiles)
    {
        if (!deleteFile(chunkFile))
        {
            std::cerr << "Failed to delete chunk file: " << chunkFile << std::endl;
            return false;
        }
    }

    return true;
}
