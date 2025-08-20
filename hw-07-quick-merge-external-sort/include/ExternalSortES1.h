#pragma once

#include "ExternalSort.h"
#include <memory>
#include "FileService.h"
#include "Sortable.h"

/**
 * @file ExternalSortES1.h
 * @brief ExternalSortES1:
 *  - splits input file into T chunks by key ranges using FileService
 *  - sorts each chunk with in-memory sorter
 *  - merges chunks by concatenation using FileService
 */
class ExternalSortES1 : public ExternalSort {
public:
    ExternalSortES1(std::shared_ptr<FileService> fileService, std::shared_ptr<Sortable> inMemSorter)
        : fs_(std::move(fileService)), inMemSorter_(std::move(inMemSorter)) {}

    bool externalSort(const std::string &inputFile, const std::string &outputFile, int T) override;

private:
    std::shared_ptr<FileService> fs_;
    std::shared_ptr<Sortable> inMemSorter_;
};
