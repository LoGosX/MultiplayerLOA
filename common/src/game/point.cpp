#include "game/point.h"

Point::Point(int r, int c) : 
    row(r), column(c) {}

bool Point::operator==(const Point& other) const {
    return row == other.row && column == other.column;
}
