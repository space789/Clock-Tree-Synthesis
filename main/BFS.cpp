///////////////////////////
// BFS.cpp               //
// Author: Shiina        //
// Date: 2024/09/30      //
// Version: 1.0          //
// copiright 2024        //
///////////////////////////

#include "BFS.h"
#include "Constants.h"
#include <queue>
#include <algorithm>
#include <cmath>
#include <climits>
#include <iostream>

std::vector<Point> BFS::leeAlgorithm(Grid& grid, const Point& start, const Point& end, bool preferVertical) {
    std::queue<Point> q;
    q.push(start);
    grid.markCell(start, 1); // Starting cell marked with 1

    // Directions: East, West, North, South
    const int dx[] = {1, -1, 0, 0}; // Horizontal moves
    const int dy[] = {0, 0, -1, 1}; // Vertical moves

    bool found = false;

    while (!q.empty() && !found) {
        Point current = q.front();
        q.pop();
        int currentStep = grid.getValue(current);

        if (current == start) {
           currentStep = 0;
        }

        // Directions for expansion
        int directions[4] = {0, 1, 2, 3}; // Default order: East, West, North, South

        // Shuffle directions for uniform exploration
        std::random_shuffle(std::begin(directions), std::end(directions));

        for (int i = 0; i < 4; ++i) {
            int dir = directions[i];
            Point next(current.x + dx[dir], current.y + dy[dir]);

            // Check if next cell is valid
            if (!grid.isValid(next)) continue;

            int nextValue = grid.getValue(next);
            if (nextValue >= 0 && nextValue <= currentStep + 1) {
                // Already visited with same or shorter path
                continue;
            }

            // Gap check
            bool canMove = true;
            // Calculate Euclidean distance between current and end point
            // double distToEnd = std::sqrt((current.x - end.x)*(current.x - end.x) + (current.y - end.y)*(current.y - end.y));
            // if (distToEnd > grid.gap) {
            //     // Perform gap check
            //     for (int g = 1; g <= grid.gap; ++g) {
            //         int lookaheadX = current.x + dx[dir] * g;
            //         int lookaheadY = current.y + dy[dir] * g;
            //         Point lookaheadPoint(lookaheadX, lookaheadY);
            //         // if (!grid.isValid(lookaheadPoint)) {
            //         //     canMove = false;
            //         //     break;
            //         // }
            //         int cellValue = grid.getValue(lookaheadPoint);
            //         if (cellValue == OBSTACLE || cellValue == WIRE || cellValue == TURN_POINT || (cellValue == CENTER && ~(lookaheadPoint == end))) {
            //             canMove = false;
            //             break;
            //         }
            //     }

            //     // std::cout<< "CHECK\n";
            // } else {
            //     // Close enough to end point; only check immediate next cell
            //     if (nextValue == OBSTACLE || nextValue == WIRE || nextValue == TURN_POINT || (nextValue == CENTER && ~(next == end))) {
            //         canMove = false;
            //     }
            // }

            if (canMove) {
                if (next == end || nextValue == EMPTY) {
                    if (next == end) {
                        found = true;
                        break;
                    }
                    grid.markCell(next, currentStep + 1);
                    q.push(next);
                }
            }
        }
    }

    // No path found
    // if (!found) return {};
    if (!found) {
        std::cout << "No path found from " << start.x << " " << start.y << " to " << end.x << " " << end.y << std::endl;
        return {};
    }
    // Backtracking to find the path
    std::vector<Point> path;
    Point current = end;
    path.push_back(current);

    while (!(current == start)) {
        int currentStep = grid.getValue(current);
        Point nextCell = current;

        // Directions for backtracking
        int directions[4];
        if (preferVertical) {
            // Prefer vertical moves (North, South)
            directions[0] = 2; // North
            directions[1] = 3; // South
            directions[2] = 0; // East
            directions[3] = 1; // West
        } else {
            // Prefer horizontal moves (East, West)
            directions[0] = 0; // East
            directions[1] = 1; // West
            directions[2] = 2; // North
            directions[3] = 3; // South
        }

        if (current == end) {
            for (int i = 0; i < 4; ++i) {
                int dir = directions[i];
                // int max_val = grid.getValue(current);
                int min_val = INT_MAX;
                Point next(current.x + dx[dir], current.y + dy[dir]);
                if (grid.getValue(next) < min_val && grid.getValue(next) >= 0) {
                    currentStep = grid.getValue(next);
                }
            }
            currentStep++;
            // std::cout << "Current step: " << currentStep << std::endl;
            // currentStep = INT_MAX;
        }


        for (int i = 0; i < 4; ++i) {
            int dir = directions[i];
            Point next(current.x + dx[dir], current.y + dy[dir]);
            if (grid.isValid(next)) {
                int nextVal = grid.getValue(next);
                if ((~(next == start) && nextVal == currentStep - 1) ||
                    (next == start)) {
                    currentStep = nextVal;
                    nextCell = next;
                    // Break early to enforce preference
                    break;
                }
            }
        }

        if (nextCell == current) {
            std::cout << "No path found during backtracking\n";
            // No path found during backtracking
            return {};
        }

        current = nextCell;
        path.push_back(current);
    }



    // if (start == Point(686, 43) || start == Point(730, 131) || start == Point(642, 37)) {
    // // print all path
    //     std::cout << "Find path from " << start.x << " " << start.y << " to " << end.x << " " << end.y << std::endl;
    //     for (int i = 0; i < path.size(); i++) {
    //         std::cout << path[i].x << " " << path[i].y << std::endl;
    //     }
    // }


    // print all grid
    // for (int y = 0; y <= grid.dimY; ++y) {
    //     for (int x = 0; x <= grid.dimX; ++x) {
    //         std::cout << grid.getValue(Point(x, y)) << " ";
    //     }
    //     std::cout << std::endl;
    // }

    std::reverse(path.begin(), path.end());
    return path;
}
