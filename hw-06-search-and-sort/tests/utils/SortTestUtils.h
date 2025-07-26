#pragma once
#include <vector>
#include <iostream>
#include "Record.h"

inline bool isSorted(const std::vector<Record>& arr) {
    for (size_t i = 1; i < arr.size(); ++i)
        if (arr[i - 1].getKey() > arr[i].getKey())
            return false;
    return true;
}

inline void printRecords(const std::vector<Record>& data) {
    std::cout << "Sort result: ";
    for (const auto& record : data) {
        std::cout << "(" << record.getKey() << ", " << record.getValue() << ") ";
    }
    std::cout << std::endl;
}
