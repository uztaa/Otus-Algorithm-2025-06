// Queen.h

#pragma once
#include "Moveable.h"

// Класс для вычисления возможных ходов королевы на шахматной доске
class Queen : public Moveable {
public:
    std::pair<int, uint64_t> getMoves(int position) const override;
};