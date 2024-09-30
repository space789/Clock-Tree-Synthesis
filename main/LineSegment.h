///////////////////////////
// LineSegment.h         //
// Author: Shiina        //
// Date: 2024/09/30      //
// Version: 1.0          //
// copiright 2024        //
///////////////////////////

#ifndef LINESEGMENT_H
#define LINESEGMENT_H

#include "Point.h"

class LineSegment {
public:
    Point start;
    Point end;
    LineSegment(const Point& start, const Point& end);
    int length() const;
};

#endif
