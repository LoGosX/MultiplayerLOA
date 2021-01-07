#pragma once

#include <vector>

#include "common/game/color.h"
#include "common/game/move.h"
#include "common/game/point.h"

class Board {
public:
    virtual std::vector<Move> GetMovesFor(Color color) const = 0;
    virtual void DoMove(Move) = 0;
    virtual Color GetColorAt(Point p) const = 0;
    virtual int GetSize() const = 0;
};