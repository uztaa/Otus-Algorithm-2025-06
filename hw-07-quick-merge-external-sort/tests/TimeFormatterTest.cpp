#include <gtest/gtest.h>
#include <string>
#include "TimeFormatter.h"


class TimeFormatterTest : public ::testing::Test {
protected:
    TimeFormatter formatter;
};

TEST_F(TimeFormatterTest, ReturnsTimeoutForNegativeInput) {
    EXPECT_EQ(formatter.formatDuration(-1), "timeout");
}

TEST_F(TimeFormatterTest, FormatsNanosecondsCorrectly) {
    EXPECT_EQ(formatter.formatDuration(0), "0 ns");
    EXPECT_EQ(formatter.formatDuration(999), "999 ns");
}

TEST_F(TimeFormatterTest, FormatsMicrosecondsCorrectly) {
    // 1000 ns = 1 us
    std::string formatted = formatter.formatDuration(1500); // 1.5 us
    EXPECT_TRUE(formatted == "1.5 µs" || formatted == "1.5 us"); // depending on encoding
}

TEST_F(TimeFormatterTest, FormatsMillisecondsCorrectly) {
    // 1_000_000 ns = 1 ms
    std::string formatted = formatter.formatDuration(1'500'000); // 1.5 ms
    EXPECT_EQ(formatted, "1.5 ms");
}

TEST_F(TimeFormatterTest, FormatsSecondsCorrectly) {
    // 1_000_000_000 ns = 1 s
    std::string formatted = formatter.formatDuration(1'500'000'000); // 1.5 s
    EXPECT_EQ(formatted, "1.5 s");
}

TEST_F(TimeFormatterTest, FormatsMinutesAndSecondsCorrectly) {
    // 70 seconds = 1 min 10 seconds = 70 * 1_000_000_000 ns
    int64_t ns = 70 * 1'000'000'000LL + 500'000'000; // 1m 10.5s
    std::string formatted = formatter.formatDuration(ns);
    EXPECT_TRUE(formatted.find("1 m ") != std::string::npos);
    EXPECT_TRUE(formatted.find("10.5 s") != std::string::npos);
}

TEST_F(TimeFormatterTest, FormatsHoursMinutesSecondsCorrectly) {
    // 1 hour 1 minute 1.5 seconds
    int64_t ns = 1 * 60 * 60 * 1'000'000'000LL + 1 * 60 * 1'000'000'000LL + 1500 * 1'000'000LL;
    // 1h 1m 1.5s
    std::string formatted = formatter.formatDuration(ns);
    EXPECT_TRUE(formatted.find("1 h ") != std::string::npos);
    EXPECT_TRUE(formatted.find("1 m ") != std::string::npos);
    EXPECT_TRUE(formatted.find("1.5 s") != std::string::npos);
}
