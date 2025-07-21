#include "Queen.h"
#include "Rook.h"
#include "Bishop.h"
#include "BitUtils.h"

std::pair<int, uint64_t> Queen::getMoves(int position) const {
    if (position < 0 || position > 63)
        return { 0, 0 };

    // Используем существующие реализации
    Rook rook;
    Bishop bishop;

    auto [rookCount, rookMask] = rook.getMoves(position);
    auto [bishopCount, bishopMask] = bishop.getMoves(position);

    uint64_t mask = rookMask | bishopMask;
    int count = BitUtils::popcount(mask);

    return { count, mask };
}
