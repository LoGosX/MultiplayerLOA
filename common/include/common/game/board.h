#pragma once

#include <vector>

#include "common/game/color.h"
#include "common/game/move.h"
#include "common/game/point.h"

class Board {
public:
    virtual std::vector<Move> GetMovesFor(Color color) const = 0;
    virtual void DoMove(Move) = 0;
    virtual Color GetColorAt(int r, int c) const = 0;
    virtual int GetSize() const = 0;
};