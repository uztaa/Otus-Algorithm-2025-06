// King.h

#pragma once
#include "Moveable.h"

// Класс для вычисления возможных ходов короля на шахматной доске
class King : public Moveable {
public:
    std::pair<int, uint64_t> getMoves(int position) const override;
};