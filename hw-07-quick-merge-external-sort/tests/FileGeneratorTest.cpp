#include <gtest/gtest.h>
#include "FileGenerator.h"
#include "InMemoryFileService.h"

TEST(FileGeneratorTest, GeneratesCorrectFileInMemory) {
    auto memFs = std::make_shared<InMemoryFileService>();
    FileGenerator gen(memFs);

    std::string testFile = "testFile.txt";
    size_t N = 50;
    int T = 100;
    uint32_t seed = 123;

    bool created = gen.generateFile(testFile, N, T, seed);
    ASSERT_TRUE(created);
    ASSERT_TRUE(memFs->fileExists(testFile));

    std::vector<int> contents;
    ASSERT_TRUE(memFs->readLines(testFile, contents));
    ASSERT_EQ(contents.size(), N);

    // Проверяем диапазон значений
    for (int val : contents) {
        EXPECT_GE(val, 1);
        EXPECT_LE(val, T);
    }
}
