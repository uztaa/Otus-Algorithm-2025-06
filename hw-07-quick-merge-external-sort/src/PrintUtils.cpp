#include "PrintUtils.h"
#include <iostream>

void printRecords(const std::vector<Record>& arr, std::ostream& os) {
    os << "[ ";
    for (std::size_t i = 0; i < arr.size(); ++i) {
        const Record& r = arr[i];
        if (!r.getValue().empty()) {
            os << '(' << r.getKey() << ", \"" << r.getValue() << "\")";
        } else {
            os << r.getKey();
        }
        if (i + 1 < arr.size()) os << ", ";
    }
    os << " ]";
}
