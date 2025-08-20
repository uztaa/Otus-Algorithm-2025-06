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

// Дополнение теста для проверки mergeChunks и splitFileToChunks

TEST(FileSystemServiceTest, SplitAndMergeChunks) {
    FileSystemService fs;
    std::string inputFile = "split_merge_test_input.txt";
    std::string outputFile = "split_merge_test_output.txt";

    // Создаем входной файл с числами (например, 30 чисел от 0 до 29)
    std::vector<int> inputData;
    for(int i = 0; i < 30; ++i) inputData.push_back(i);
    ASSERT_TRUE(fs.writeLines(inputFile, inputData));

    int T = 3;
    int maxKey = 29;
    std::vector<std::string> chunkFiles;

    // Разбиваем файл на 3 частей
    ASSERT_TRUE(fs.splitFileToChunks(inputFile, T, maxKey, chunkFiles));
    ASSERT_EQ(chunkFiles.size(), 3);

    // Проверяем, что chunk файлы существуют и содержат ожидаемые данные
    for (const auto& chunkFile : chunkFiles) {
        ASSERT_TRUE(fs.fileExists(chunkFile));
    }

    // Объединяем чанки обратно в outputFile
    ASSERT_TRUE(fs.mergeChunks(chunkFiles, outputFile));
    ASSERT_TRUE(fs.fileExists(outputFile));

    // Считаем итоговый файл и сверяем с исходными данными (в данном случае просто конкатенация)
    std::vector<int> mergedData;
    ASSERT_TRUE(fs.readLines(outputFile, mergedData));
    ASSERT_EQ(mergedData.size(), inputData.size());

    // Т.к. данные в чанках лежат по ключам, итоговый файл будет частично отсортирован по чанкам
    // Проверим что все элементы присутствуют (можно отсортировать для проверки)
    std::sort(mergedData.begin(), mergedData.end());
    std::vector<int> sortedInputData = inputData;
    std::sort(sortedInputData.begin(), sortedInputData.end());
    ASSERT_EQ(mergedData, sortedInputData);

    // Удаляем временные файлы
    for (const auto& chunkFile : chunkFiles) {
        fs.deleteFile(chunkFile);
    }
    fs.deleteFile(inputFile);
    fs.deleteFile(outputFile);
}
