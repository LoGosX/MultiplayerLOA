#include "game/move.h"

Move::Move(Point src, Point dst) :
    src_(src), dst_(dst) {}

Point Move::GetSource() const {
    return src_;
}

Point Move::GetDestination() const {
    return dst_;
}