#pragma once

struct Point {
    int row = -1;
    int column = -1;
    Point() = default;
    Point(int row, int column);

    bool operator==(const Point &) const;
};