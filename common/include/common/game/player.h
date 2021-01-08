#pragma once
#include "common/game/color.h"
#include "common/game/move.h"

class Board;

class Player {
public:
    Color GetColor() const;
    Move GetMove() const;
    void Think(Board * board);
    bool IsReady();
};