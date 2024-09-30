///////////////////////////
// BFS.h                 //
// Author: Shiina        //
// Date: 2024/09/30      //
// Version: 1.0          //
// copiright 2024        //
///////////////////////////

#ifndef BFS_H
#define BFS_H

#include <vector>
#include "Point.h"
#include "Grid.h"

class BFS {
public:
    static std::vector<Point> leeAlgorithm(Grid& grid, const Point& start, const Point& end, bool preferVertical);
};

#endif
