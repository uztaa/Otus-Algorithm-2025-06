#pragma once

#include "ExternalSort.h"
#include <memory>
#include "FileService.h"
#include "Sortable.h"

/**
 * @brief Внешняя сортировка по методу ES3:
 *   - загрузка файла блоками по 100 чисел,
 *   - внутренняя сортировка каждого блока,
 *   - запись отсортированных блоков во временный файл,
 *   - затем внешний цикл слияния как в ES2.
 */
class ExternalSortES3 : public ExternalSort {
public:
    ExternalSortES3(std::shared_ptr<FileService> fileService, std::shared_ptr<Sortable> inMemSorter)
        : fs_(std::move(fileService)), inMemSorter_(std::move(inMemSorter)) {}

    bool externalSort(const std::string& inputFile, const std::string& outputFile,
                      int number_of_chunks, int maxValue) override;

private:
    static constexpr size_t BLOCK_SIZE = 100;

    std::shared_ptr<FileService> fs_;
    std::shared_ptr<Sortable> inMemSorter_;
};
