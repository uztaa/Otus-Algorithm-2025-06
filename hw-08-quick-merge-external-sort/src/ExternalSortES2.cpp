#include "ExternalSortES2.h"
#include "Record.h"
#include <vector>
#include <iostream>

/**
 * @brief Реализация внешней сортировки с двумя вспомогательными файлами
 * Алгоритм:
 *  - Считать весь входной файл в память
 *  - Отсортировать внутренним сортировщиком (run длины 1)
 *  - Записать отсортированный массив во временный файл temp1
 *  - Итеративно сливать блоки runLength из temp1 и temp2 в другой файл, удваивая runLength после каждого прохода
 *  - Когда runLength >= размера данных, результат в одном из temp-файлов — переименовать в outputFile
 */
bool ExternalSortES2::externalSort(const std::string& inputFile,
                                   const std::string& outputFile,
                                   int /*number_of_chunks*/,
                                   int /*maxValue*/)
{
    const std::string tempFile1 = "temp1.txt";
    const std::string tempFile2 = "temp2.txt";

    // 1. Считать весь входной файл в память (вектор int)
    std::vector<int> data;
    if (!fs_->readLines(inputFile, data)) {
        std::cerr << "Failed to read input file: " << inputFile << std::endl;
        return false;
    }

    size_t n = data.size();
    if (n == 0) {
        // Если файл пуст, просто создаем пустой выходной файл
        return fs_->createFile(outputFile);
    }

    // 2. Отсортировать весь массив внутренним сортировщиком
    std::vector<Record> records;
    records.reserve(n);
    for (int val : data) {
        records.emplace_back(val, "");
    }
    inMemSorter_->sort(records);

    // Конвертируем обратно в int для записи
    std::vector<int> sortedData;
    sortedData.reserve(n);
    for (const auto& r : records) {
        sortedData.push_back(r.getKey());
    }

    // Записываем отсортированный массив в tempFile1
    if (!fs_->writeLines(tempFile1, sortedData)) {
        std::cerr << "Failed to write sorted data to temp file: " << tempFile1 << std::endl;
        return false;
    }

    // 3. Итеративное слияние с удвоением длины run'а
    // Начинаем с runLength=1 — длина блоков, которые сливаем
    size_t runLength = 1;

    // На каждой итерации читаем из одного файла и пишем в другой
    // Чередуем tempFile1 и tempFile2 как источник и приемник
    bool finished = false;

    while (!finished) {
        std::string inputA, inputB, output;

        // Определяем, какой файл читать, а в какой писать
        // Чередование:
        // Нечетные итерации: читаем из tempFile1 и tempFile2, пишем в tempFile2
        // Четные итерации: наоборот
        if ((runLength & 1) == 1) {
            inputA = tempFile1;
            inputB = tempFile2;
            output = tempFile2;
        } else {
            inputA = tempFile2;
            inputB = tempFile1;
            output = tempFile1;
        }

        // Считываем содержимое файлов
        std::vector<int> fileA, fileB;
        bool readA = fs_->readLines(inputA, fileA);
        bool readB = fs_->readLines(inputB, fileB);

        if (!readA) fileA.clear();
        if (!readB) fileB.clear();

        size_t indexA = 0, indexB = 0;
        size_t sizeA = fileA.size();
        size_t sizeB = fileB.size();

        std::vector<int> mergedResult;
        mergedResult.reserve(sizeA + sizeB);

        // Сливаем блоками runLength из fileA и fileB
        while (indexA < sizeA || indexB < sizeB) {
            size_t endA = std::min(indexA + runLength, sizeA);
            size_t endB = std::min(indexB + runLength, sizeB);

            size_t posA = indexA;
            size_t posB = indexB;

            // Слияние двух отсортированных run'ов
            while (posA < endA && posB < endB) {
                if (fileA[posA] <= fileB[posB]) {
                    mergedResult.push_back(fileA[posA++]);
                } else {
                    mergedResult.push_back(fileB[posB++]);
                }
            }

            // Добавляем остатки из первого run
            while (posA < endA) {
                mergedResult.push_back(fileA[posA++]);
            }

            // Добавляем остатки из второго run
            while (posB < endB) {
                mergedResult.push_back(fileB[posB++]);
            }

            indexA += runLength;
            indexB += runLength;
        }

        // Записываем результат слияния
        if (!fs_->writeLines(output, mergedResult)) {
            std::cerr << "Failed to write merged data to temporary file: " << output << std::endl;
            return false;
        }

        // Если длина run покрывает весь массив — сортировка завершена
        if (mergedResult.size() == n) {
            finished = true;

            // Если итоговый результат не совпадает с именем outputFile,
            // копируем или переименовываем
            if (output != outputFile) {
                // Сначала попытка удалить, чтобы не было коллизии
                fs_->deleteFile(outputFile);

                // Пытаемся переименовать файл, если FileService поддерживает это (например, FileSystemService)
                // В интерфейсе FileService нет rename, будем делать через удаление и запись
                // Читаем данные из output и записываем в outputFile
                std::vector<int> finalData;
                if (!fs_->readLines(output, finalData)) {
                    std::cerr << "Failed to read final sorted file: " << output << std::endl;
                    return false;
                }
                if (!fs_->writeLines(outputFile, finalData)) {
                    std::cerr << "Failed to write final sorted file: " << outputFile << std::endl;
                    return false;
                }
                fs_->deleteFile(output);
            }
        } else {
            // Удваиваем длину run для следующей итерации
            runLength *= 2;
        }
    }

    return true;
}
