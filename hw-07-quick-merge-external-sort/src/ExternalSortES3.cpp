#include "ExternalSortES3.h"
#include "Record.h"
#include <vector>
#include <string>
#include <iostream>


bool ExternalSortES3::externalSort(const std::string& inputFile,
                                   const std::string& outputFile,
                                   int /*number_of_chunks*/,
                                   int /*maxValue*/)
{
    const std::string tempFile1 = "es3_temp_1.txt";
    const std::string tempFile2 = "es3_temp_2.txt";

    // 1. Read input fully
    std::vector<int> inputData;
    if (!fs_->readLines(inputFile, inputData)) {
        std::cerr << "Failed to read input file: " << inputFile << "\n";
        return false;
    }

    if (inputData.empty())
        return fs_->createFile(outputFile); // empty output for empty input

    // 2. Break into sorted blocks (size 100) and write concatenated sorted blocks to tempFile1
    if (!fs_->createFile(tempFile1)) {
        std::cerr << "Failed to create temp file " << tempFile1 << "\n";
        return false;
    }

    size_t pos = 0;
    const size_t totalSize = inputData.size();

    while (pos < totalSize) {
        size_t blockEnd = std::min(pos + BLOCK_SIZE, totalSize);
        std::vector<Record> records;
        records.reserve(blockEnd - pos);
        for (size_t i = pos; i < blockEnd; ++i) {
            records.emplace_back(inputData[i], "");
        }
        inMemSorter_->sort(records);

        std::vector<int> sortedBlock;
        sortedBlock.reserve(records.size());
        for (const auto& r : records)
            sortedBlock.push_back(r.getKey());

        if (!fs_->appendLines(tempFile1, sortedBlock)) {
            std::cerr << "Failed to append block to " << tempFile1 << "\n";
            return false;
        }
        pos = blockEnd;
    }

    // 3. Iterative merge of runs
    size_t runLength = BLOCK_SIZE;
    bool finished = false;
    bool toggle = false; // To switch between tempFile1 and tempFile2 for reading/writing

    while (!finished) {
        const std::string& inputFileName = toggle ? tempFile2 : tempFile1;
        const std::string& outputFileName = toggle ? tempFile1 : tempFile2;

        std::vector<int> data;
        if (!fs_->readLines(inputFileName, data)) {
            std::cerr << "Failed to read temp file " << inputFileName << "\n";
            return false;
        }

        if (data.size() <= runLength) {
            // Done merging, write to outputFile if needed
            if (outputFileName != outputFile) {
                if (!fs_->writeLines(outputFile, data)) {
                    std::cerr << "Failed to write final output file\n";
                    return false;
                }
            } else {
                if (!fs_->writeLines(outputFileName, data)) {
                    std::cerr << "Failed to write final output file\n";
                    return false;
                }
            }
            finished = true;
            break;
        }

        std::vector<int> merged;
        merged.reserve(data.size());

        size_t i = 0;
        size_t n = data.size();

        // Merge pairs of runs of size runLength
        while (i < n) {
            size_t left = i;
            size_t mid = std::min(i + runLength, n);
            size_t right = std::min(i + 2 * runLength, n);

            size_t li = left;
            size_t ri = mid;

            // Merge runs [left, mid) and [mid, right)
            while (li < mid && ri < right) {
                if (data[li] <= data[ri]) {
                    merged.push_back(data[li++]);
                } else {
                    merged.push_back(data[ri++]);
                }
            }
            while (li < mid) {
                merged.push_back(data[li++]);
            }
            while (ri < right) {
                merged.push_back(data[ri++]);
            }
            i += 2 * runLength;
        }

        if (!fs_->writeLines(outputFileName, merged)) {
            std::cerr << "Failed to write merged chunk to " << outputFileName << "\n";
            return false;
        }

        runLength *= 2;
        toggle = !toggle; // Swap input/output for next iteration
    }

    // Clean up temp files
    fs_->deleteFile(tempFile1);
    fs_->deleteFile(tempFile2);

    return true;
}
