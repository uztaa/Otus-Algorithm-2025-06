// FileSystemServiceTests.cpp
#include <gtest/gtest.h>
#include "FileSystemService.h"
#include <fstream>
#include <filesystem>
#include "FileGenerator.h"
#include "InMemoryFileService.h"

// удалить данные или вывести в лог информацию об ошибке
void tryDeleteOrLog(const std::shared_ptr<FileService>& fs, const std::string& path) {
     if( !fs->deleteFile(path) ) {
            std::cerr << "not deleted " << path << std::endl;
        }
}

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
    // Arrange
    std::string inputFile = "split_merge_test_input.txt";
    std::string outputFile = "split_merge_test_output.txt";

    int chunck_size = 3;
    int maxKey = 15;
    int rows = 5;
    uint32_t seed = 123;

    auto realFs = std::make_shared<FileSystemService>(); // <=  InMemoryFileService
    FileGenerator gen(realFs);
     
    bool created = gen.generateFile(inputFile, rows, maxKey, seed);

    // Act
    std::vector<std::string> chunkFiles;
    bool result = realFs->splitFileToChunks(inputFile, chunck_size, maxKey, chunkFiles);

    // Assert
    ASSERT_TRUE(result);
    ASSERT_EQ(chunkFiles.size(), chunck_size);

    // Проверяем, что chunk файлы существуют и содержат ожидаемые данные
    for (const auto& chunkFile : chunkFiles) {
        ASSERT_TRUE(realFs->fileExists(chunkFile));
    }

    // Объединяем чанки обратно в outputFile
    ASSERT_TRUE(realFs->mergeChunks(chunkFiles, outputFile));
    ASSERT_TRUE(realFs->fileExists(outputFile));

    // Считаем итоговый файл и сверяем с исходными данными (в данном случае просто конкатенация)
    std::vector<int> mergedData;
    ASSERT_TRUE(realFs->readLines(outputFile, mergedData));
    ASSERT_EQ(mergedData.size(), rows);

    tryDeleteOrLog(realFs, inputFile);
    tryDeleteOrLog(realFs, outputFile);
}
