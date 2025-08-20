#include <gtest/gtest.h>
#include <memory>
#include <vector>
#include <string>

#include "ExternalSortES1.h"
#include "FileSystemService.h"
#include "InMemoryFileService.h"
#include "QuickSorter.h"
#include "RandomArrayGenerator.h"
#include "FileGenerator.h"

class ExternalSortES1Test : public ::testing::Test {
protected:
    void SetUp() override {
        fs = std::make_shared<FileSystemService>(); // InMemoryFileService
        sorter = std::make_shared<QuickSorter>();
        externalSorter = std::make_unique<ExternalSortES1>(fs, sorter);

        inputFile = "test_input.txt";
        outputFile = "test_output.txt";

        FileGenerator fg(fs);
        size_t N = 100;
        T = 10;
        seed = 12345;

        ASSERT_TRUE(fg.generateFile(inputFile, N, T, seed));
    }

    void TearDown() override {
        // Cleanup files
        fs->deleteFile(inputFile);
        fs->deleteFile(outputFile);
        for (int i = 0; i < T; ++i) {
            std::string chunkFile = "chunk_" + std::to_string(i) + ".txt";
            fs->deleteFile(chunkFile);
        }
    }

    std::shared_ptr<FileService> fs;
    std::shared_ptr<QuickSorter> sorter;
    std::unique_ptr<ExternalSortES1> externalSorter;
    std::string inputFile;
    std::string outputFile;
    int T;
    uint32_t seed;
};


TEST_F(ExternalSortES1Test, ExternalSort_SortsCorrectly) {
    bool res = externalSorter->externalSort(inputFile, outputFile, T);
    ASSERT_TRUE(res);

    std::vector<int> sortedKeys;

    ASSERT_TRUE(fs->readLines(outputFile, sortedKeys));
    ASSERT_FALSE(sortedKeys.empty());

    // Check sorted ascending order
    for (size_t i = 1; i < sortedKeys.size(); ++i) {
        ASSERT_LE(sortedKeys[i-1], sortedKeys[i]);
    }
}

TEST_F(ExternalSortES1Test, ExternalSort_InvalidT_Fails) {
    bool res = externalSorter->externalSort(inputFile, outputFile, 0);
    ASSERT_FALSE(res);

    res = externalSorter->externalSort(inputFile, outputFile, -1);
    ASSERT_FALSE(res);
}

TEST_F(ExternalSortES1Test, ExternalSort_NonExistentInput_Fails) {
    bool res = externalSorter->externalSort("none_existing_file.txt", outputFile, T);
    ASSERT_FALSE(res);
}
