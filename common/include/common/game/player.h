#pragma once
#include "common/game/color.h"

class Player {
public:
    virtual Color GetColor() const = 0;
    virtual Move GetMove() = 0;
};