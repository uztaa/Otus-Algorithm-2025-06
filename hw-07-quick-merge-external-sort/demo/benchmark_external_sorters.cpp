#include <iostream>
#include <iomanip>
#include <memory>
#include <string>
#include <vector>
#include <chrono>
#include <future>

#include "FileSystemService.h"
#include "FileGenerator.h"
#include "ExternalSortES1.h"
#include "ExternalSortES2.h"
#include "ExternalSortES3.h"
#include "QuickSorter.h"
#include "TimeFormatter.h"

static const std::vector<size_t> NS = {100, 1000, 10'000, 100'000, 1'000'000};
static const uint32_t SEED = 12345u;

static int64_t NS_IN_US = 1000;               // микросекунда в нс
static const int64_t NS_IN_MS = 1'000'000;    // милисекунда
static const int64_t NS_IN_S = 1'000'000'000; // секунда
static const int64_t NS_IN_M = 60 * NS_IN_S;  // минута
static const int64_t NS_IN_H = 60 * NS_IN_M;  // час

static const int64_t TIMEOUT = 30 * NS_IN_M; // таймаут выполнения работы

// Структура для хранения одного результата
struct BenchmarkResult
{
    size_t N;
    size_t T;
    std::string sorter; // ES1 / ES2 / ES3
    int64_t durationMs;
    bool ok;
};

// Запуск externalSort с таймаутом; возвращает (успех, длительность мс)
template <class ES>
std::pair<bool, long> runWithTimeout(ES &es,
                                     const std::string &in,
                                     const std::string &out,
                                     int chunks,
                                     int maxValue,
                                     int64_t timeout_ms)
{
    auto start = std::chrono::steady_clock::now();

    auto fut = std::async(std::launch::async, [&]()
                          { return es.externalSort(in, out, chunks, maxValue); });

    if (fut.wait_for(std::chrono::nanoseconds(timeout_ms)) >= std::future_status::timeout)
    {
        return {false, -1};
    }

    bool ok = fut.get();
    auto end = std::chrono::steady_clock::now();
    long dur = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    return {ok, dur};
}

// Проверка, что файл отсортирован
bool isFileSorted(FileService &fs, const std::string &filename)
{
    std::vector<int> data;
    if (!fs.readLines(filename, data))
        return false;
    for (size_t i = 1; i < data.size(); ++i)
        if (data[i - 1] > data[i])
            return false;
    return true;
}

// Один прогон для пары (N, T, sorterType)
BenchmarkResult runSingleBenchmark(size_t N, size_t T, const std::string &sorterType)
{
    auto fs = std::make_shared<FileSystemService>();
    auto inMem = std::make_shared<QuickSorter>();

    const std::string input = "input.txt";
    const std::string output = "output.txt";

    FileGenerator gen(fs);
    gen.generateFile(input, N, static_cast<int>(T), SEED);

    bool ok = false;
    long duration = -1;

    if (sorterType == "ES1")
    {
        ExternalSortES1 es(fs, inMem);
        int chunks = std::max<int>(1, static_cast<int>(N / 100));
        auto [res, dur] = runWithTimeout(es, input, output, chunks, static_cast<int>(T), TIMEOUT);
        ok = res && isFileSorted(*fs, output);
        duration = dur;
    }
    else if (sorterType == "ES2")
    {
        ExternalSortES2 es(fs, inMem);
        auto [res, dur] = runWithTimeout(es, input, output, 0, static_cast<int>(T), TIMEOUT);
        ok = res && isFileSorted(*fs, output);
        duration = dur;
    }
    else if (sorterType == "ES3")
    {
        ExternalSortES3 es(fs, inMem);
        auto [res, dur] = runWithTimeout(es, input, output, 0, 0, TIMEOUT);
        ok = res && isFileSorted(*fs, output);
        duration = dur;
    }

    return BenchmarkResult{N, T, sorterType, duration, ok};
}

// Вывод таблицы
void printResults(const std::vector<BenchmarkResult> &results)
{
    std::cout << std::left
              << std::setw(10) << "N"
              << std::setw(10) << "T"
              << std::setw(8) << "Sorter"
              << std::setw(14) << "Time"
              << "\n";
    std::cout << std::string(50, '=') << "\n";

    TimeFormatter tf;

    for (const auto &r : results)
    {
        std::cout << std::left
                  << std::setw(10) << r.N
                  << std::setw(10) << r.T
                  << std::setw(8) << r.sorter
                  << std::setw(14) << tf.formatDuration(r.durationMs)
                  << "\n";
    }
}

int main()
{
    std::vector<BenchmarkResult> results;
    for (size_t N : NS)
    {
        for (size_t T : {size_t(10), N})
        {
            results.push_back(runSingleBenchmark(N, T, "ES1"));
            results.push_back(runSingleBenchmark(N, T, "ES2"));
            results.push_back(runSingleBenchmark(N, T, "ES3"));
        }
    }
    printResults(results);
    return 0;
}
