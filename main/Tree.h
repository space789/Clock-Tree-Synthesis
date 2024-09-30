///////////////////////////
// Tree.h                //
// Author: Shiina        //
// Date: 2024/09/30      //
// Version: 1.0          //
// copiright 2024        //
///////////////////////////

#ifndef TREE_H
#define TREE_H

#include <vector>
#include <memory>
#include "Point.h"
#include "Node.h"

class Tree {
public:
    std::unique_ptr<Node> root;
    const int axis_first = 0;

    // Constructor: Initializes the Tree object.
    Tree();

    // Builds the tree using the provided points.
    void buildTree(std::vector<Point>& points);

    // Rebuilds the tree to fix routing issues by adjusting the subtrees of problematic nodes.
    void rebuildTree(Node* node, std::vector<std::pair<Point, Point>>& errorPaths, const int casetype);

    // Prints the tree structure for debugging purposes.
    void printTree(Node* node, int depth = 0);

private:
    // Recursively builds the tree from the points, alternating split axis at each depth.
    std::unique_ptr<Node> buildTreeRecursive(std::vector<Point>& points, int depth);

    // Calculates the mean (center) point from a set of points.
    Point calculateMean(const std::vector<Point>& points);

    // Calculates the center point based on the depth and splitting axis.
    Point calculateCenter(std::vector<Point>& points, const int depth);

    // Calculates the Euclidean distance between two points.
    double calculateDistance(const Point& a, const Point& b);

    // Calculates the depth of the tree.
    int calculateDepth(Node* node);

    // Adjusts the leaf nodes based on the depth to balance the tree.
    void adjustLeafNodes(std::unique_ptr<Node>& node, const int depth);

    // // Checks if a node with the given center exists in the subtree.
    // bool isNodeInSubtree(Node* subtree, const Point& targetCenter);

    // // Collects all leaf nodes under a given node for point redistribution.
    // void collectLeafNodes(Node* node, std::vector<Node*>& leafNodes);
};

#endif // TREE_H
