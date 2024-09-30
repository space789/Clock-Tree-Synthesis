///////////////////////////
// Grid.cpp              //
// Author: Shiina        //
// Date: 2024/09/30      //
// Version: 1.0          //
// copiright 2024        //
///////////////////////////

#include "Grid.h"
#include "Constants.h"
#include <algorithm>

Grid::Grid(int dimX, int dimY, int gap) : dimX(dimX), dimY(dimY), gap(gap) {
    grid.resize(dimX + 1, std::vector<int>(dimY + 1, EMPTY));
}

void Grid::markCell(const Point& p, int value) {
    if (isValid(p)) {
        grid[p.x][p.y] = value;
    }
}

int Grid::getValue(const Point& p) const {
    if (isValid(p)) {
        return grid[p.x][p.y];
    }
    return OBSTACLE; // Out of bounds or invalid cell
}

bool Grid::isValid(const Point& p) const {
    return p.x >= 0 && p.x <= dimX && p.y >= 0 && p.y <= dimY;
}

void Grid::reset() {
    for (int x = 0; x <= dimX; ++x) {
        for (int y = 0; y <= dimY; ++y) {
            int value = grid[x][y];
            if (value >= 0) { // Reset wavefront markers
                grid[x][y] = EMPTY;
            }
        }
    }
}

void Grid::resetall() {
    for (int x = 0; x <= dimX; ++x) {
        for (int y = 0; y <= dimY; ++y) {
            grid[x][y] = EMPTY;
        }
    }
}
