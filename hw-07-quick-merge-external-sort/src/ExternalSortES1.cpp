#include "ExternalSortES1.h"
#include "FileService.h"
#include "Sortable.h"
#include "RandomArrayGenerator.h"
#include "Record.h"
#include <vector>
#include <string>
#include <iostream>

bool ExternalSortES1::externalSort(const std::string& inputFile, const std::string& outputFile, int number_of_chunks, int maxValue)
{
    if (number_of_chunks <= 0)
    {
        std::cerr << "Parameter number_of_chunks must be positive\n";
        return false;
    }

    std::vector<std::string> chunkFiles;

    // Разбиваем файл на T чанков по ключам
    if (!fs_->splitFileToChunks(inputFile, number_of_chunks, maxValue, chunkFiles))
    {
        std::cerr << "splitFileToChunks failed\n";
        return false;
    }

    // Сортируем каждый chunk внутренним сортировщиком
    for (const auto &chunkFile : chunkFiles)
    {
        std::vector<int> chunkData;
        if (!fs_->readLines(chunkFile, chunkData))
        {
            std::cerr << "Failed to read chunk file: " << chunkFile << "\n";
            return false;
        }

        std::vector<Record> records;
        records.reserve(chunkData.size());
        for (const auto &v : chunkData)
            records.emplace_back(v, "");

        inMemSorter_->sort(records);

        // Перезаписываем chunk отсортированными ключами
        chunkData.clear();
        for (const auto &r : records)
            chunkData.push_back(r.getKey());

        if (!fs_->writeLines(chunkFile, chunkData))
        {
            std::cerr << "Failed to write sorted chunk file: " << chunkFile << "\n";
            return false;
        }
    }

    // Объединяем отсортированные чанки в результирующий файл (реализовано в FileService)
    bool mergeResult = fs_->mergeChunks(chunkFiles, outputFile);

    if (!mergeResult)
    {
        std::cerr << "Merged chunks error " << "\n";
    }

    return mergeResult;
}
