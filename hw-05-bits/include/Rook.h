// Rook.h

#pragma once
#include "Moveable.h"

// Класс для вычисления возможных ходов ладьи на шахматной доске
class Rook : public Moveable {
public:
    std::pair<int, uint64_t> getMoves(int position) const override;
};