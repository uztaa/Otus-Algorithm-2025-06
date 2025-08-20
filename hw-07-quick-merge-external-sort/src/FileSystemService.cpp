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
