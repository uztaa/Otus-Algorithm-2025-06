#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <memory>
#include <thread>
#include <mutex>
#include <map>
#include <string>
#include <iomanip>
#include "../include/Sortable.h"
#include "../src/sort/BubbleSorter.cpp"
#include "../src/sort/InsertionSorter.cpp"
#include "../src/sort/CocktailSorter.cpp"
#include "../src/sort/InsertionShiftSorter.cpp"
#include "../src/sort/BinaryInsertionSorter.cpp"
#include "../src/sort/CustomShellSorter.cpp"
#include "ProgressBarListener.h"

using namespace std;
using namespace chrono;

// Глобальные переменные должны быть объявлены заранее
vector<size_t> sizes = { 100, 1000 }; //100, 1000 , 10000, 100000, 1000000
map<string, map<size_t, long long>> results;
mutex resultMutex;

// Создание случайного массива заданного размера
vector<Record> generateRandomData(size_t size) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, 1000000);
    vector<Record> vec;
    for (size_t i = 0; i < size; ++i) {
        vec.emplace_back(dist(gen), "x");
    }
    return vec;
}

// Бенчмарк одного алгоритма на всех размерах, используя копии общего массива
void benchmark(const string name, unique_ptr<Sortable> sorter, const vector<size_t> sizes,
    shared_ptr<map<size_t, vector<Record>>> originalData,
    std::shared_ptr<ProgressBarListener> progress_bar_listener)
{
    sorter->setName(name);
    // Устанавливаем слушателя прогресс-бара для сортировщика
    sorter->addListener(progress_bar_listener);


    for (size_t sz : sizes) {
        auto it = originalData->find(sz);
        if (it == originalData->end()) {
            cerr << "ERROR: Size " << sz << " not found in originalData map. Skipping...\n";
            continue;
        }

        vector<Record> copy = it->second;

        auto start = chrono::high_resolution_clock::now();
        try {
            sorter->sort(copy);
        } catch (const exception& e) {
            cerr << "Exception during sorting with " << name << ": " << e.what() << endl;
            continue; // Пропустить этот размер, если произошла ошибка
		}
        auto end = chrono::high_resolution_clock::now();

        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        lock_guard<mutex> lock(resultMutex);
        results[name][sz] = duration;
    }
}

// Метод печати результатов бенчмарка
void printResults() {
    using std::cout;
    using std::endl;
    using std::setw;
    using std::left;

    const std::string COLOR_HEADER = "\033[1;34m";
    const std::string COLOR_CELL = "\033[0;36m";
    const std::string COLOR_RESET = "\033[0m";

    // ── Вычисление динамической ширины столбцов ──────────────────
    size_t algoNameWidth = std::string("Algorithm").size();
    size_t valueWidth = 8;//std::string("Time(ms)").size();

    for (const auto& [algoName, dataMap] : results) {
        algoNameWidth = std::max(algoNameWidth, algoName.size());
        for (const auto& [size, time] : dataMap) {
            valueWidth = std::max(valueWidth, std::to_string(time).size());
        }
    }

    algoNameWidth += 2; // запас
    valueWidth = std::max<size_t>(10, valueWidth + 2); // минимум 10 символов + отступ

    // ── Верхняя граница таблицы ───────────────────────────────────
    cout << COLOR_HEADER << "+" << std::string(algoNameWidth + 2, '-') << "+";
    for (size_t sz : sizes) {
        cout << std::string(valueWidth + 2, '-') << "+";
    }
    cout << endl;

    // ── Заголовок ─────────────────────────────────────────────────
    cout << "| " << setw(algoNameWidth) << left << "Algorithm" << " |";
    for (size_t sz : sizes) {
        cout << " " << setw(valueWidth) << sz << " |";
    }
    cout << endl;

    // ── Разделитель ───────────────────────────────────────────────
    cout << "+" << std::string(algoNameWidth + 2, '-') << "+";
    for (size_t sz : sizes) {
        cout << std::string(valueWidth + 2, '-') << "+";
    }
    cout << COLOR_RESET << endl;

    // ── Данные ────────────────────────────────────────────────────
    for (const auto& [algoName, dataMap] : results) {
        cout << COLOR_CELL << "| " << setw(algoNameWidth) << left << algoName << " |";
        for (size_t sz : sizes) {
            auto it = dataMap.find(sz);
            if (it != dataMap.end()) {
                cout << " " << setw(valueWidth) << it->second << " |";
            }
            else {
                cout << " " << setw(valueWidth) << "n/a" << " |";
            }
        }
        cout << COLOR_RESET << endl;
    }

    // ── Нижняя граница ────────────────────────────────────────────
    cout << COLOR_HEADER << "+" << std::string(algoNameWidth + 2, '-') << "+";
    for (size_t sz : sizes) {
        cout << std::string(valueWidth + 2, '-') << "+";
    }
    cout << COLOR_RESET << endl;
}



// Основная функция, которая запускает бенчмарки
int main() {
    auto start_benchmark = chrono::high_resolution_clock::now();

    cout << "random data generating..."  << endl;
	// Генерация случайных данных для каждого размера
    auto originalDataPtr = make_shared<map<size_t, vector<Record>>>();

    for (size_t sz : sizes) {
        (*originalDataPtr)[sz] = generateRandomData(sz);
    }

    cout << "benchmark running... (please, wait)" << endl << endl;
	
    // Запуск бенчмарков в отдельных потоках
    vector<thread> threads;

    // вычисляем ожидаемое дискретное время выполнения N*N
	// для прогресс-бара, чтобы он мог корректно отображать прогресс
    auto N = sizes[sizes.size() - 1]; // Используем последний размер для инициализации слушателя
	// Создаем слушателя для прогресс-бара
    auto progress_bar_listener = nullptr;
    //auto progress_bar_listener = std::make_shared<ProgressBarListener>(N * N);
    
    threads.emplace_back(benchmark, "BubbleSort", make_unique<BubbleSorter>(), sizes, originalDataPtr, progress_bar_listener);
    threads.emplace_back(benchmark, "InsertionSort", make_unique<InsertionSorter>(), sizes, originalDataPtr, progress_bar_listener);
    threads.emplace_back(benchmark, "CocktailSorter", make_unique<CocktailSorter>(), sizes, originalDataPtr, progress_bar_listener);
    threads.emplace_back(benchmark, "InsertionShiftSorter", make_unique<InsertionShiftSorter>(), sizes, originalDataPtr, progress_bar_listener);
    threads.emplace_back(benchmark, "BinaryInsertionSorter", make_unique<BinaryInsertionSorter>(), sizes, originalDataPtr, progress_bar_listener);

    // алгоритмы ShellSort с разными реализациями стратегии выбора шагов.
    threads.emplace_back(benchmark, "ShellSorter.Classic", make_unique<CustomShellSorter>(CustomShellSorter::GapStrategy::Classic), sizes, originalDataPtr, progress_bar_listener);
    threads.emplace_back(benchmark, "ShellSorter.Hibbard", make_unique<CustomShellSorter>(CustomShellSorter::GapStrategy::Hibbard), sizes, originalDataPtr, progress_bar_listener);
    threads.emplace_back(benchmark, "ShellSorter.Sedgewick", make_unique<CustomShellSorter>(CustomShellSorter::GapStrategy::Sedgewick), sizes, originalDataPtr, progress_bar_listener);


    for (auto& t : threads) {
        t.join();
    }

    auto end_benchmark = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::minutes>(end_benchmark - start_benchmark).count();

    std::cout << "\nBenchmark duration: " << duration << " minutes" << std::endl;
    printResults();

    return 0;
}

/*
Benchmark results:
+-------------------------+------------+------------+------------+------------+
| Algorithm               | 100        | 1000       | 10000      | 100000     |
+-------------------------+------------+------------+------------+------------+
| BinaryInsertionSorter   | 0          | 58         | 2069       | 202828     |
| BubbleSort              | 11         | 1907       | 75781      | 9962441    |
| CocktailSorter          | 22         | 1988       | 76110      | 10078481   |
| InsertionShiftSorter    | 0          | 52         | 2406       | 236521     |
| InsertionSort           | 45         | 1911       | 73615      | 9811897    |
| ShellSorter.Classic     | 3          | 71         | 1183       | 16227      |
| ShellSorter.Hibbard     | 8          | 43         | 1103       | 14184      |
| ShellSorter.Sedgewick   | 1          | 82         | 808        | 8708       |
+-------------------------+------------+------------+------------+------------+
P.S. Time in milliseconds.
*/
