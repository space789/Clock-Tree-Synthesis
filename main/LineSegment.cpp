///////////////////////////
// LineSegment.cpp       //
// Author: Shiina        //
// Date: 2024/09/30      //
// Version: 1.0          //
// copiright 2024        //
///////////////////////////

#include "LineSegment.h"
#include <cmath>

LineSegment::LineSegment(const Point& start, const Point& end) : start(start), end(end) {}

int LineSegment::length() const {
    return std::abs(end.x - start.x) + std::abs(end.y - start.y);
}
