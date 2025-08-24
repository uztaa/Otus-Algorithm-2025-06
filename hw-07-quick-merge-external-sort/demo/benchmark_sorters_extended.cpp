#include <thread>
#include <iostream>
#include <iomanip>
#include <memory>
#include <vector>
#include <string>
#include <chrono>
#include <future>
#include <map>
#include <mutex>

#include <fstream>
#include <filesystem>

#include "QuickSorter.h"
#include "MergeSorter.h"
#include "BenchmarkDataGenerator.h"
#include "TimeFormatter.h"
#include "TimeSettings.h"
#include "Timer.h"

#include "SorterFactory.h"

// seed для воспроизводимости бенчмарка и равных условиях для всех сортировщиков
constexpr uint32_t SEED = 12345u;

// таймаут на выполнение одной сортировки
// временем на создание данных и сохранением в файл можно принебречь
constexpr int64_t TIMEOUT_NS = 1 * NS_IN_M; 

// путь, куда будут сохраняться файлы
const std::string PATH_TO_SAVE_FILES = "/media/anton/Acer/test";
// флаг включения сохранения отсортированных файлов
const bool SAVE_FILE_ENABLE = false;

const std::vector<size_t> SIZES = {1, 10, 100, 1'000, 10'000, 100'000}; //, 1'000'000};
const std::vector<std::string> DATA_TYPES = {"random", "digits", "sorted", "revers"};
const std::vector<std::string> SORTERS = SorterFactory::getAvailableSorters();

// конвертер наносекунд в читабельную строку
TimeFormatter tf;

// Функция генерации данных единым способом с использованием BenchmarkDataGenerator и seed
std::vector<Record> generateDataByType(BenchmarkDataGenerator &gen, size_t size, const std::string &dataType)
{
    std::vector<Record> data;
    if (dataType == "random")
    {
        data = gen.random(size);
    }
    else if (dataType == "digits")
    {
        data = gen.digits(size);
    }
    else if (dataType == "sorted")
    {
        // 1% dirties по условию задачи
        size_t dirties = static_cast<size_t>(size * 0.01);
        data = gen.sorted(size, dirties);
    }
    else if (dataType == "revers")
    {
        data = gen.revers(size);
    }
    return data;
}

// Структура для идентификации запроса
struct BenchmarkRequest
{
    size_t size;
    std::string dataType;
    std::string sorterName;

    bool operator<(const BenchmarkRequest &other) const
    {
        return std::tie(size, dataType, sorterName) < std::tie(other.size, other.dataType, other.sorterName);
    }
};

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
    std::cout << std::left
              << std::setw(12) << "Size"
              << std::setw(10) << "Type"
              << std::setw(32) << "Sorter"
              << std::setw(20) << "Duration"
              << std::setw(10) << "Success"
              << "\n";
    std::cout << std::string(84, '=') << "\n";

    for (const auto &r : results)
    {
        std::cout << std::left
                  << std::setw(12) << r.size
                  << std::setw(10) << r.dataType
                  << std::setw(32) << r.sorterName
                  << std::setw(20) << tf.formatDuration(r.durationNs)
                  << std::setw(10) << (r.success ? "yes" : "no")
                  << "\n";
    }
}

// создаем запросы на формирование бенчмарка
std::vector<BenchmarkRequest> createBenchmarkRequests()
{
    std::vector<BenchmarkRequest> requests;
    for (auto size : SIZES)
    {
        for (const auto &dataType : DATA_TYPES)
        {
            for (const auto &sorterName : SORTERS)
            {
                BenchmarkRequest req{size, dataType, sorterName};
                requests.push_back(req);
            }
        }
    }

    return requests;
}

// сохраняет отсортированные данные в файл
void saveSortedDataToFile(const std::vector<Record> &data, const BenchmarkRequest &req)
{
    namespace fs = std::filesystem;

    // Формируем имя файла
    std::string filename = std::to_string(req.size) + "_" + req.dataType + "_" + req.sorterName + ".txt";
    fs::path filepath = fs::path(PATH_TO_SAVE_FILES) / filename;

    // Создаем директорию, если ее нет
    fs::create_directories(filepath.parent_path());

    std::ofstream ofs(filepath);
    if (!ofs.is_open())
    {
        throw std::runtime_error("Failed to open file for writing: " + filepath.string());
    }

    // Первая строка - размер массива
    ofs << data.size() << "\n";

    // Вторая строка - ключи через пробел
    for (size_t i = 0; i < data.size(); ++i)
    {
        ofs << data[i].getKey();
        if (i + 1 < data.size())
            ofs << " ";
    }
    ofs << "\n";
}


int main()
{
    std::cout << "benchmark started" << std::endl;
    Timer benchmarkTimer;
    benchmarkTimer.start();

    BenchmarkDataGenerator gen;

    std::vector<BenchmarkRequest> requests = createBenchmarkRequests();

    std::mutex resultsMutex; // mutex для добавления результатов из разных потоков
    std::vector<BenchmarkResult> allResults;

    std::map<BenchmarkRequest, std::future<void>> futures;
    for (auto req : requests)
    {
        futures[req] = std::async(std::launch::async, [&, req]()
                                  {
                                        std::unique_ptr<Sortable> sorter = SorterFactory::createSorter(req.sorterName);
                                        std::vector<Record> data = generateDataByType(gen, req.size, req.dataType);

                                              Timer timer;
                                              timer.start();
                                              sorter->sort(data);
                                              timer.stop();

                                              BenchmarkResult res{req.size, req.dataType, req.sorterName, true, timer.getDurationNs()};
                                              std::lock_guard<std::mutex> lock(resultsMutex);
                                              allResults.push_back(res);

                                            
                                             // сохранение файлов
                                             if (SAVE_FILE_ENABLE) {
                                                timer.start();
                                                saveSortedDataToFile(data, req);
                                                timer.stop();
                                                std::cout << "save file duration=" << tf.formatDuration(timer.getDurationNs()) << std::endl;
                                             }

                                            });
    }


    /**
     * Время ожидания (нс) future.
     * Изначально совпадает с TIMEOUT_NS.
     * На каждом извлечении future уменьшается на время выполнения конкретного future.
    */ 
    int64_t wait_time = TIMEOUT_NS;

    // Ждем завершения всех задач
    for (auto &[req, fut] : futures)
    {
        try
        {
            std::cout << "wait_time=" << tf.formatDuration(wait_time) << std::endl;
            if (fut.wait_for(std::chrono::nanoseconds(wait_time)) == std::future_status::timeout)
            {
                // выбрасываем исключение, чтобы прервать дальнейшую работу
                throw std::runtime_error("Timeout");
            }            
            fut.get();

            // делаем пересчте ожидания следующего future
            std::lock_guard<std::mutex> lock(resultsMutex);
            for(auto &r: allResults) {
                wait_time -= r.durationNs;
            if (wait_time <= 0) wait_time = 100; // Если уходим в минус, то присваиваем мальнькое значение 100 нс.
            }  
        }
        catch (const std::exception &e)
        {
            BenchmarkResult res{req.size, req.dataType, req.sorterName, false, -1};
            std::lock_guard<std::mutex> lock(resultsMutex);
            allResults.push_back(res);
        }
    }

    benchmarkTimer.stop();
    std::cout << "benchmark finished at " 
        << tf.formatDuration(benchmarkTimer.getDurationNs()) << std::endl;

    printResults(allResults);

    return 0;
}
