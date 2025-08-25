#pragma once

#include <string>

/**
 * @file 
 * Interface for external sorting algorithms.
 */
class ExternalSort {
public:
    virtual ~ExternalSort() = default;

    /**
     * Sort inputFile externally with parameter T and write sorted result to outputFile.
     * @param inputFile path to input file.
     * @param outputFile path to output sorted file.
     * @param maxValue max of value
     * @param number_of_chunks parameter controlling sorting behavior (e.g., number of chunks).
     * @return true if sorting succeeded, false otherwise.
     */
    virtual bool externalSort(const std::string& inputFile, const std::string& outputFile, int number_of_chunks, int maxValue) = 0;
};
