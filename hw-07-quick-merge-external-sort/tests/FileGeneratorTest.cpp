#include <gtest/gtest.h>
#include "FileGenerator.h"
#include "InMemoryFileService.h"
#include "FileSystemService.h"

TEST(FileGeneratorTest, GeneratesCorrectFileInMemory)
{
    // Arrange
    auto memFs = std::make_shared<InMemoryFileService>(); // работает и с FileSystemService
    FileGenerator gen(memFs);

    std::string testFile = "FileGeneratorTest.txt";
    size_t rows = 5;
    int maxValue = 15;
    uint32_t seed = 123;

    // Act
    bool created = gen.generateFile(testFile, rows, maxValue, seed);

    // Assert
    ASSERT_TRUE(created);
    ASSERT_TRUE(memFs->fileExists(testFile));

    std::vector<int> contents;
    ASSERT_TRUE(memFs->readLines(testFile, contents));

    std::vector<int> expected = {3, 12, 14, 5, 5}; // зависит от MIN_DISTRIBUTION, MAX_DISTRIBUTION
    ASSERT_EQ(expected, contents);

    ASSERT_TRUE(memFs->deleteFile(testFile));
}
