#include <gtest/gtest.h>
#include <chrono>
#include <thread>
#include <utility>
#include "AsyncRunner.h"

TEST(AsyncRunnerTest, TaskCompletesSuccessfullyWithinTimeout) {
    AsyncRunner runner;

    auto [success, duration] = runner.asyncRunWithTimeout([]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return true;
    }, std::chrono::milliseconds(500).count() * 1000000); // 500 ms in ns

    EXPECT_TRUE(success);
    EXPECT_GT(duration, 0);
}

TEST(AsyncRunnerTest, TaskTimeout) {
    AsyncRunner runner;

    auto [success, duration] = runner.asyncRunWithTimeout([]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        return true;
    }, std::chrono::milliseconds(100).count() * 1000000); // 100 ms in ns timeout

    EXPECT_FALSE(success);
    EXPECT_EQ(duration, -1);
}

TEST(AsyncRunnerTest, TaskReturnsFalse) {
    AsyncRunner runner;

    auto [success, duration] = runner.asyncRunWithTimeout([]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        return false;
    }, std::chrono::milliseconds(200).count() * 1000000); // 200 ms in ns timeout

    EXPECT_FALSE(success);
    EXPECT_GT(duration, 0);
}
