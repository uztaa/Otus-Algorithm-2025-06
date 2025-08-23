#include <thread>
#include <iostream>
#include <iomanip>
#include <memory>
#include <vector>
#include <string>
#include <chrono>
#include <future>

#include "QuickSorter.h"
#include "MergeSorter.h"
#include "BenchmarkDataGenerator.h"
#include "TimeFormatter.h"
#include <AsyncRunner.h>

constexpr uint32_t SEED = 12345u;
constexpr int64_t SECOND = 1'000'000'000; // одна секунда в наносекундах
constexpr int64_t TIMEOUT_NS = 1LL * 1 * SECOND; // таймаут на выполнение одной сортировки

const std::vector<size_t> SIZES = {1, 10, 100, 1'000, 10'000};//, 100'000}; //, 1'000'000};
const std::vector<std::string> DATA_TYPES = {"random", "digits", "sorted", "revers"};

// Функция генерации данных единым способом с использованием BenchmarkDataGenerator и seed
std::vector<Record> generateDataByType(BenchmarkDataGenerator &gen, size_t size, const std::string &dataType)
{
    if (dataType == "random")
    {
        return gen.random(size);
    }
    else if (dataType == "digits")
    {
        return gen.digits(size);
    }
    else if (dataType == "sorted")
    {
        // 1% dirties по условию задачи
        size_t dirties = static_cast<size_t>(size * 0.01);
        return gen.sorted(size, dirties);
    }
    else if (dataType == "revers")
    {
        return gen.revers(size);
    }
    else
    {
        return {};
    }
}

template <typename Sorter>
std::pair<bool, long> trySort(BenchmarkDataGenerator &gen, Sorter &sorter, size_t size, const std::string &dataType, int64_t /*timeout_ns*/)
{
    std::vector<Record> data = generateDataByType(gen, size, dataType);

    auto start = std::chrono::steady_clock::now();
    sorter.sort(data);
    auto end = std::chrono::steady_clock::now();
    long duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

    // Проверяем что отсортировано по ключу
    for (size_t i = 1; i < data.size(); ++i)
    {
        if (data[i - 1].getKey() > data[i].getKey())
        {
            return {false, duration_ns};
        }
    }

    return {true, duration_ns};
}

struct BenchmarkResult
{
    size_t size;
    std::string dataType;
    std::string sorterName;
    bool success;
    int64_t durationNs;
};

void printResults(const std::vector<BenchmarkResult> &results)
{
    TimeFormatter tf;

    std::cout << std::left
              << std::setw(12) << "Size"
              << std::setw(10) << "Type"
              << std::setw(12) << "Sorter"
              << std::setw(20) << "Duration"
              << std::setw(8) << "Success"
              << "\n";

    std::cout << std::string(62, '=') << "\n";

    for (const auto &r : results)
    {
        std::cout << std::left
                  << std::setw(12) << r.size
                  << std::setw(10) << r.dataType
                  << std::setw(12) << r.sorterName
                  << std::setw(20) << tf.formatDuration(r.durationNs)
                  << std::setw(8) << (r.success ? "yes" : "no")
                  << "\n";
    }
}

int main()
{
    BenchmarkDataGenerator gen;

    std::vector<BenchmarkResult> allResults;
    std::mutex resultsMutex;
    std::vector<std::future<void>> futures;
    AsyncRunner runner;

    for (auto size : SIZES)
    {
        for (const auto &dataType : DATA_TYPES)
        {
            // QuickSorter task
            futures.push_back(std::async(std::launch::async, [&gen, size, &dataType, &allResults, &resultsMutex, &runner]()
                                         {
                QuickSorter sorter;
                auto task = [&gen, &sorter, size, &dataType]() {
                    auto [success, dur] = trySort(gen, sorter, size, dataType, TIMEOUT_NS);
                    return success;
                };
                auto [success, dur] = runner.asyncRunWithTimeout(task, TIMEOUT_NS);
                BenchmarkResult res = {size, dataType, sorter.getName(), success, dur};
                {
                    std::lock_guard<std::mutex> lock(resultsMutex);
                    allResults.push_back(std::move(res));
                }

                printResults({allResults.back()}); // промежуточное логирование
                std::cout << "------------------\n"; }));

            // MergeSorter task
            futures.push_back(std::async(std::launch::async, [&gen, size, &dataType, &allResults, &resultsMutex, &runner]()
                                         {
                MergeSorter sorter;
                auto task = [&gen, &sorter, size, &dataType]() {
                    auto [success, dur] = trySort(gen, sorter, size, dataType, TIMEOUT_NS);
                    return success;
                };
                auto [success, dur] = runner.asyncRunWithTimeout(task, TIMEOUT_NS);
                BenchmarkResult res = {size, dataType, sorter.getName(), success, dur};
                {
                    std::lock_guard<std::mutex> lock(resultsMutex);
                    allResults.push_back(std::move(res));
                }

                printResults({allResults.back()}); // промежуточное логирование
                std::cout << "------------------\n"; }));
        }
    }

    // Ждем завершения всех задач
    for (auto &fut : futures)
        fut.get();

    std::cout << "\n-------- done ----------\n\n";

    printResults(allResults);
    return 0;
}
