#include <gtest/gtest.h>
#include <memory>
#include <vector>
#include <string>

#include "ExternalSortES2.h"
#include "FileSystemService.h"
#include "InMemoryFileService.h"
#include "QuickSorter.h"
#include "RandomArrayGenerator.h"
#include "FileGenerator.h"

class ExternalSortES2Test : public ::testing::Test {
protected:
    void SetUp() override {
        // Используем FileSystemService, можно поменять на InMemoryFileService для тестов без файловой системы
        fs = std::make_shared<InMemoryFileService>();
        sorter = std::make_shared<QuickSorter>();
        externalSorter = std::make_unique<ExternalSortES2>(fs, sorter);

        inputFile = "test_input.txt";
        outputFile = "test_output.txt";

        FileGenerator fg(fs);
        N = 1000;           // Количество строк
        maxValue = 100;     // Максимальное значение
        seed = 12345;

        ASSERT_TRUE(fg.generateFile(inputFile, N, maxValue, seed));
    }

    void TearDown() override {
        // FileSystemService удаляет свои папки в деструкторе
    }

    std::shared_ptr<FileService> fs;
    std::shared_ptr<QuickSorter> sorter;
    std::unique_ptr<ExternalSortES2> externalSorter;
    std::string inputFile;
    std::string outputFile;
    int maxValue;
    uint32_t seed;
    size_t N;
};

// Проверка корректной сортировки
TEST_F(ExternalSortES2Test, ExternalSort_SortsCorrectly) {
    // number_of_chunks здесь не используется в ES2, передадим для совместимости 0
    bool res = externalSorter->externalSort(inputFile, outputFile, 0, maxValue);
    ASSERT_TRUE(res);

    std::vector<int> sortedKeys;
    ASSERT_TRUE(fs->readLines(outputFile, sortedKeys));
    ASSERT_FALSE(sortedKeys.empty());

    // Проверяем, что отсортировано по возрастанию
    for (size_t i = 1; i < sortedKeys.size(); ++i) {
        ASSERT_LE(sortedKeys[i - 1], sortedKeys[i]);
    }
}

// Проверка обработки несуществующего входного файла
TEST_F(ExternalSortES2Test, ExternalSort_NonExistentInput_Fails) {
    bool res = externalSorter->externalSort("non_existing_file.txt", outputFile, 0, maxValue);
    ASSERT_FALSE(res);
}

// Проверка обработки пустого файла сортировкой (создает пустой выход)
TEST_F(ExternalSortES2Test, ExternalSort_EmptyInputFile_CreatesEmptyOutput) {
    std::string emptyFile = "empty_input.txt";
    fs->createFile(emptyFile);

    bool res = externalSorter->externalSort(emptyFile, outputFile, 0, maxValue);
    ASSERT_TRUE(res);

    std::vector<int> outData;
    ASSERT_TRUE(fs->readLines(outputFile, outData));
    ASSERT_TRUE(outData.empty());

    fs->deleteFile(emptyFile);
}
