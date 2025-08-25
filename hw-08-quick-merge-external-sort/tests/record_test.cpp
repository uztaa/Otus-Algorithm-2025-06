#include <gtest/gtest.h>
#include "Record.h"

TEST(RecordTest, KeyAccess) {
    Record r(42, "example");
    EXPECT_EQ(r.getKey(), 42);
    EXPECT_EQ(r.getValue(), "example");
}
