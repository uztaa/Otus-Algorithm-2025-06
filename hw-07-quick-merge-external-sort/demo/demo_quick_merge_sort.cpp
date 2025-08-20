#include <iostream>
#include <memory>
#include <vector>
#include "MergeSorter.h"
#include "QuickSorter.h"
#include "RandomArrayGenerator.h"
#include "LoggingSortListener.h"
#include "PrintUtils.h"

constexpr uint32_t SEED = 12345u;
constexpr size_t N = 20;

int main()
{
    RandomArrayGenerator gen;
    // Метод regenerate(N, SEED) генерирует одни и те же данные для фиксированных N, SEED.
    // Такми образом разные сортировщики находятся в равных условиях.
    std::vector<Record> data = gen.regenerate(N, SEED);

    std::cout << "init data:\n";
    printRecords(data, std::cout);
    std::cout << "==================================\n";

    {
        auto sorter = std::make_unique<QuickSorter>();
        sorter->addListener(std::make_shared<LoggingSortListener>());
        sorter->sort(data);
    }

    std::cout << "after first sort data:\n";
    printRecords(data, std::cout);
    std::cout << "==================================\n";

    data.clear(); 
    data.shrink_to_fit();

    std::cout << "after clear data:\n";
    printRecords(data, std::cout);
    std::cout << "==================================\n";

    // Генерируем точно такой же массив для чистоты сравнения
    data = gen.regenerate(N, SEED);
    std::cout << "second generate data:\n";
    printRecords(data, std::cout);
    std::cout << "==================================\n";

   
    {
        auto sorter = std::make_unique<MergeSorter>();
        sorter->addListener(std::make_shared<LoggingSortListener>());
        sorter->sort(data);
    }

    std::cout << "after second sort data:\n";
    printRecords(data, std::cout);
    std::cout << "==================================\n";

    // Проверка, что отсортировано по возрастанию ключей
    bool ok = true;
    for (std::size_t i = 1; i < data.size(); ++i)
        ok &= (data[i - 1].getKey() <= data[i].getKey());
    std::cout << (ok ? "OK" : "FAIL") << std::endl;
}