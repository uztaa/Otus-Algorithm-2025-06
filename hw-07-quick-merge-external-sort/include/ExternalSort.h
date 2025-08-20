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
     * @param T parameter controlling sorting behavior (e.g., number of chunks).
     * @return true if sorting succeeded, false otherwise.
     */
    virtual bool externalSort(const std::string& inputFile, const std::string& outputFile, int T) = 0;
};
