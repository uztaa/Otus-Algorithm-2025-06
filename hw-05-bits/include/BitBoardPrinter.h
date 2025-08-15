// BitBoardPrinter.h

#pragma once
#include <cstdint>
#include <iostream>

// This class provides a utility to print a bitboard in a human-readable format.
class BitBoardPrinter {
public:
    static void print(uint64_t bitboard);
};
