#include "FileSystemService.h"
#include <fstream>
#include <filesystem>
#include <iostream>

bool FileSystemService::createFile(const std::string& path) {
    std::ofstream ofs(path, std::ios::trunc);
    return ofs.is_open();
}

bool FileSystemService::writeLines(const std::string& path, const std::vector<int>& lines) {
    std::ofstream ofs(path, std::ios::trunc);
    if (!ofs.is_open()) return false;
    for (int val : lines) {
        ofs << val << '\n';
    }
    return true;
}

bool FileSystemService::appendLines(const std::string& path, const std::vector<int>& lines) {
    std::ofstream ofs(path, std::ios::app);
    if (!ofs.is_open()) return false;
    for (int val : lines) {
        ofs << val << '\n';
    }
    return true;
}

bool FileSystemService::readLines(const std::string& path, std::vector<int>& outLines) {
    std::ifstream ifs(path);
    if (!ifs.is_open()) return false;
    outLines.clear();
    int val;
    while (ifs >> val) {
        outLines.push_back(val);
    }
    return true;
}

bool FileSystemService::deleteFile(const std::string& path) {
    std::error_code ec;
    bool removed = std::filesystem::remove(path, ec);
    if (!removed) {
        std::cerr << "Failed to delete file: " << path << ", error: " << ec.message() << std::endl;
    }
    return removed;
}

bool FileSystemService::fileExists(const std::string& path) {
    return std::filesystem::exists(path);
}

bool FileSystemService::splitFileToChunks(const std::string& inputFile, int T, int maxKey, std::vector<std::string>& chunkFiles) {
    chunkFiles.clear();
    // Создаем T пустых файлов chunk_0.txt,... и добавляем их в chunkFiles
    for (int i = 0; i < T; ++i) {
        std::string filename = "chunk_" + std::to_string(i) + ".txt";
        if (!createFile(filename)) {
            std::cerr << "Failed to create chunk file: " << filename << std::endl;
            return false;
        }
        chunkFiles.push_back(filename);
    }

    std::ifstream ifs(inputFile);
    if (!ifs.is_open()) {
        std::cerr << "Failed to open input file: " << inputFile << std::endl;
        return false;
    }

    std::vector<std::ofstream> ofsVec;
    for (const auto& file : chunkFiles) {
        std::ofstream ofs(file, std::ios::app);
        if (!ofs.is_open()) {
            std::cerr << "Failed to open chunk file: " << file << std::endl;
            return false;
        }
        ofsVec.push_back(std::move(ofs));
    }

    int interval = (maxKey + T - 1) / T;
    int val;
    while (ifs >> val) {
        int idx = val / interval;
        if (idx >= T) idx = T - 1;
        ofsVec[idx] << val << '\n';
    }

    return true;
}

bool FileSystemService::mergeChunks(const std::vector<std::string>& chunkFiles, const std::string& outputFile) {
    std::ofstream ofs(outputFile, std::ios::trunc);
    if (!ofs.is_open()) {
        std::cerr << "Failed to open output file: " << outputFile << std::endl;
        return false;
    }

    for (const auto& file : chunkFiles) {
        std::ifstream ifs(file);
        if (!ifs.is_open()) {
            std::cerr << "Failed to open chunk file: " << file << std::endl;
            return false;
        }
        ofs << ifs.rdbuf();
    }
    return true;
}
