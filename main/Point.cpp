///////////////////////////
// File: Point.cpp       //
// Author: Shiina        //
// Date: 2024/09/30      //
// Version: 1.0          //
// copiright 2024        //
///////////////////////////

#include "Point.h"

Point::Point(int x, int y) : x(x), y(y) {}

bool Point::operator==(const Point& other) const {
    return x == other.x && y == other.y;
}

bool Point::operator<(const Point& other) const {
    if (x != other.x) return x < other.x;
    return y < other.y;
}
