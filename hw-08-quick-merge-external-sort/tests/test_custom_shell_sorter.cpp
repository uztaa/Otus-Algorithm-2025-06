#include <gtest/gtest.h>
#include "CustomShellSorter.h"
#include "utils/SortTestUtils.h"

class CustomShellSorterTest : public ::testing::Test {
protected:
    std::vector<Record> baseData = {
        {5, "e"}, {1, "a"}, {3, "c"}, {4, "d"}, {2, "b"}
    };

    void runTest(CustomShellSorter::GapStrategy strategy) {
        std::vector<Record> data = baseData;
        CustomShellSorter sorter(strategy);
        sorter.sort(data);
        ASSERT_TRUE(isSorted(data));
    }
};

TEST_F(CustomShellSorterTest, ClassicStrategySortsCorrectly) {
    runTest(CustomShellSorter::GapStrategy::Classic);
}

TEST_F(CustomShellSorterTest, HibbardStrategySortsCorrectly) {
    runTest(CustomShellSorter::GapStrategy::Hibbard);
}

TEST_F(CustomShellSorterTest, SedgewickStrategySortsCorrectly) {
    runTest(CustomShellSorter::GapStrategy::Sedgewick);
}

TEST(CustomShellSorterEdgeTest, HandlesEmptyArray) {
    std::vector<Record> data;
    CustomShellSorter sorter;
    
    sorter.sort(data);
    
    EXPECT_TRUE(isSorted(data));

    //EXPECT_EQ(cmp, 0);
    //EXPECT_EQ(swp, 0);
}

TEST(CustomShellSorterEdgeTest, HandlesSingleElement) {
    std::vector<Record> data = { {1, "a"} };
    CustomShellSorter sorter;
    
    sorter.sort(data);
    
    EXPECT_TRUE(isSorted(data));
    //EXPECT_EQ(cmp, 0);
    //EXPECT_EQ(swp, 0);
}

TEST(CustomShellSorterEdgeTest, HandlesSortedArray) {
    std::vector<Record> data = { {1, "a"}, {2, "b"}, {3, "c"}, {4, "d"} };
    CustomShellSorter sorter;
    
    sorter.sort(data);
    
    EXPECT_TRUE(isSorted(data));
    //EXPECT_EQ(swp, 0);
}

TEST(CustomShellSorterEdgeTest, HandlesReversedArray) {
    std::vector<Record> data = { {4, "d"}, {3, "c"}, {2, "b"}, {1, "a"} };
    CustomShellSorter sorter;
    
    sorter.sort(data);
    
    EXPECT_TRUE(isSorted(data));
}

TEST(CustomShellSorterEdgeTest, HandlesDuplicates) {
    std::vector<Record> data = { {2, "a"}, {3, "b"}, {2, "c"}, {1, "d"}, {3, "e"} };
    CustomShellSorter sorter;
    
    sorter.sort(data);
    
    EXPECT_TRUE(isSorted(data));
}

TEST(CustomShellSorterEdgeTest, HandlesAllEqualElements) {
    std::vector<Record> data = { {1, "a"}, {1, "b"}, {1, "c"} };
    CustomShellSorter sorter;
    
    sorter.sort(data);
    
    EXPECT_TRUE(isSorted(data));
    //EXPECT_EQ(swp, 0);
}
