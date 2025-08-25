#include "gtest/gtest.h"
#include <thread>
#include <vector>
#include <set>
#include <mutex>
#include "ThreadSafeVector.h"

TEST(ThreadSafeVectorTest, ConcurrentPushBackInt) {
    ThreadSafeVector<int> tsVector;

    const int numThreads = 10;
    const int itemsPerThread = 100;

    std::vector<std::thread> threads;
    for (int t = 0; t < numThreads; ++t) {
        threads.emplace_back([t, &tsVector, itemsPerThread]() {
            for (int i = 0; i < itemsPerThread; ++i) {
                tsVector.push_back(t * itemsPerThread + i);
            }
        });
    }

    for (auto& th : threads) {
        th.join();
    }

    auto results = tsVector.getCopy();
    EXPECT_EQ(results.size(), numThreads * itemsPerThread);

    std::set<int> unique(results.begin(), results.end());
    EXPECT_EQ(unique.size(), results.size());

    for (int t = 0; t < numThreads; ++t) {
        int sample = t * itemsPerThread;
        EXPECT_TRUE(unique.find(sample) != unique.end());
    }
}
