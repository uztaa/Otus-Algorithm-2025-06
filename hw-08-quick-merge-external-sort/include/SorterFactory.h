#pragma once

#include <memory>
#include <vector>
#include <string>

#include "QuickSorter.h"
#include "MergeSorter.h"
#include "BinaryInsertionSorter.h"
#include "CocktailSorter.h"
#include "InsertionSorter.h"
#include "BubbleSorter.h"
#include "InsertionShiftSorter.h"
#include "CustomShellSorter.h"


/** 
 * @file SorterFactory.h
 * @brief Фабрика сортировщиков.
 */
class SorterFactory {
public:
    static std::vector<std::string> getAvailableSorters() {
        return {
            "QuickSorter",
            "MergeSorter",
            "BinaryInsertionSorter",
            "CocktailSorter",
            "InsertionSorter",
            "BubbleSorter",
            "InsertionShiftSorter",
            "ShellSorter.Classic",
            "ShellSorter.Hibbard",
            "ShellSorter.Sedgewick"
        };
    };

    static std::unique_ptr<Sortable> createSorter(const std::string& name) {
        if (name == "QuickSorter") return std::make_unique<QuickSorter>();
        if (name == "MergeSorter") return std::make_unique<MergeSorter>();
        
        if (name == "BinaryInsertionSorter") return std::make_unique<BinaryInsertionSorter>();
        if (name == "CocktailSorter") return std::make_unique<CocktailSorter>();
        if (name == "InsertionSorter") return std::make_unique<InsertionSorter>();
        if (name == "BubbleSorter") return std::make_unique<BubbleSorter>();
        if (name == "InsertionShiftSorter") return std::make_unique<InsertionShiftSorter>();

        if (name == "ShellSorter.Classic") return std::make_unique<CustomShellSorter>(CustomShellSorter::GapStrategy::Classic);
        if (name == "ShellSorter.Hibbard") return std::make_unique<CustomShellSorter>(CustomShellSorter::GapStrategy::Hibbard);
        if (name == "ShellSorter.Sedgewick") return std::make_unique<CustomShellSorter>(CustomShellSorter::GapStrategy::Sedgewick);
        
        return nullptr;
    }
};
