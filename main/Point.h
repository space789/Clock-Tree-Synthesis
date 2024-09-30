///////////////////////////
// Point.h               //
// Author: Shiina        //
// Date: 2024/09/30      //
// Version: 1.0          //
// copiright 2024        //
///////////////////////////

#ifndef POINT_H
#define POINT_H

struct Point {
    int x, y;
    Point(int x = 0, int y = 0);
    bool operator==(const Point& other) const;
    bool operator<(const Point& other) const;  // Added for std::map key comparison
};

#endif
