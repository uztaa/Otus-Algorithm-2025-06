#include <gtest/gtest.h>
#include <memory>
#include <vector>
#include <string>

#include "ExternalSortES3.h"
#include "FileSystemService.h"
#include "InMemoryFileService.h"
#include "QuickSorter.h"
#include "RandomArrayGenerator.h"
#include "FileGenerator.h"

class ExternalSortES3Test : public ::testing::Test {
protected:
    void SetUp() override {
        fs = std::make_shared<InMemoryFileService>();
        sorter = std::make_shared<QuickSorter>();
        externalSorter = std::make_unique<ExternalSortES3>(fs, sorter);

        inputFile = "test_input.txt";
        outputFile = "test_output.txt";

        FileGenerator fg(fs);
        N = 1000;
        maxValue = 100;
        seed = 12345;

        ASSERT_TRUE(fg.generateFile(inputFile, N, maxValue, seed));
    }

    std::shared_ptr<FileService> fs;
    std::shared_ptr<QuickSorter> sorter;
    std::unique_ptr<ExternalSortES3> externalSorter;
    std::string inputFile;
    std::string outputFile;
    int maxValue;
    uint32_t seed;
    size_t N;
};

TEST_F(ExternalSortES3Test, ExternalSort_SortsCorrectly) {
    bool res = externalSorter->externalSort(inputFile, outputFile, 0, maxValue);
    ASSERT_TRUE(res);

    std::vector<int> sortedKeys;
    ASSERT_TRUE(fs->readLines(outputFile, sortedKeys));
    ASSERT_FALSE(sortedKeys.empty());

    for (size_t i = 1; i < sortedKeys.size(); ++i) {
        ASSERT_LE(sortedKeys[i - 1], sortedKeys[i]);
    }
}

TEST_F(ExternalSortES3Test, ExternalSort_NonExistentInput_Fails) {
    bool res = externalSorter->externalSort("non_existing_file.txt", outputFile, 0, maxValue);
    ASSERT_FALSE(res);
}

TEST_F(ExternalSortES3Test, ExternalSort_EmptyInputFile_CreatesEmptyOutput) {
    std::string emptyFile = "empty_input.txt";
    fs->createFile(emptyFile);

    bool res = externalSorter->externalSort(emptyFile, outputFile, 0, maxValue);
    ASSERT_TRUE(res);

    std::vector<int> outData;
    ASSERT_TRUE(fs->readLines(outputFile, outData));
    ASSERT_TRUE(outData.empty());

    fs->deleteFile(emptyFile);
}
