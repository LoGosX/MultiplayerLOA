#pragma once

#include "common/game/point.h"

class Move {
public:
    Move() = default;
    Move(Point source, Point destination);
    Point GetSource() const;
    Point GetDestination() const;
private:
    Point src_, dst_;
};