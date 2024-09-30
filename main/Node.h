///////////////////////////
// Node.h                //
// Author: Shiina        //
// Date: 2024/09/30      //
// Version: 1.0          //
// copiright 2024        //
///////////////////////////

#ifndef NODE_H
#define NODE_H

#include <vector>
#include <memory>
#include "Point.h"

class Node {
public:
    Point center;
    std::unique_ptr<Node> left;    // Changed to smart pointers
    std::unique_ptr<Node> right;   // Changed to smart pointers
    std::vector<Point> points;
    bool isLeaf;
    Node();
    // No need for destructor; std::unique_ptr handles memory
};

#endif
