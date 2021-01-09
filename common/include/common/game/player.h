#pragma once
#include "common/game/color.h"
#include "common/game/move.h"

class Board;

class Player {
public:
    virtual Color GetColor() const = 0;
    virtual Move GetMove() = 0;
    virtual void Think(Board * board) = 0;
    virtual bool IsReady();
};