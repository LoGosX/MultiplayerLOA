#pragma once

#include <vector>

#include "include/color.h"


class Board {
public:
    virtual Color GetCurrentColor() const = 0;
    virtual std::vector<Move> GetMovesFor(Color color) const = 0;
    //virtual bool IsGameOver() const = 0;
}