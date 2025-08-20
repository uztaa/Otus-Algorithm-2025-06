// FileSystemServiceTests.cpp
#include <gtest/gtest.h>
#include "FileSystemService.h"
#include <fstream>
#include <filesystem>

TEST(FileSystemServiceTest, CreateWriteReadDeleteFile)
{
    FileSystemService fs;
    std::string filename = "fs_test_file.txt";

    // Создаем файл
    ASSERT_TRUE(fs.createFile(filename));

    // Проверяем существование файла
    ASSERT_TRUE(fs.fileExists(filename));

    // Пишем данные
    std::vector<int> data = {1, 2, 3, 4, 5};
    ASSERT_TRUE(fs.writeLines(filename, data));

    // Читаем созданный файл
    std::vector<int> readData;
    ASSERT_TRUE(fs.readLines(filename, readData));
    ASSERT_EQ(data, readData);

    // Добавляем новые данные
    std::vector<int> appendPart = {6, 7};
    ASSERT_TRUE(fs.appendLines(filename, appendPart));

    // Читаем дополненный файл
    std::vector<int> expectedRead = {1, 2, 3, 4, 5, 6, 7};
    readData.clear();
    ASSERT_TRUE(fs.readLines(filename, readData));
    ASSERT_EQ(expectedRead, readData);

    // Удаляем файл
    ASSERT_TRUE(fs.deleteFile(filename));
    ASSERT_FALSE(fs.fileExists(filename));
}
