///////////////////////////
// Grid.h                //
// Author: Shiina        //
// Date: 2024/09/30      //
// Version: 1.0          //
// copiright 2024        //
///////////////////////////

#ifndef GRID_H
#define GRID_H

#include <vector>
#include "Point.h"

class Grid {
public:
    int dimX, dimY;
    int gap;
    std::vector<std::vector<int>> grid;
    Grid(int dimX, int dimY, int gap);
    void markCell(const Point& p, int value);
    int getValue(const Point& p) const;
    bool isValid(const Point& p) const;
    void reset();
    void resetall();
};

#endif
