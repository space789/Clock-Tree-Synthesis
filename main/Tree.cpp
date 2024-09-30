///////////////////////////
// Tree.cpp              //
// Author: Shiina        //
// Date: 2024/09/30      //
// Version: 1.0          //
// copiright 2024        //
///////////////////////////

#include "Tree.h"
#include "Utilities.h"
#include <iostream>
#include <algorithm>
#include <cmath>

Tree::Tree() : root(nullptr) {}

// Builds the tree from a list of points
void Tree::buildTree(std::vector<Point>& points) {
    root = buildTreeRecursive(points, 0);
    int depth = calculateDepth(root.get());
    adjustLeafNodes(root, depth);
}

// Calculates the depth of the tree recursively
int Tree::calculateDepth(Node* node) {
    if (!node) return 0;
    return 1 + std::max(calculateDepth(node->left.get()), calculateDepth(node->right.get()));
}

// Calculates the center point based on the mean of the points
Point Tree::calculateCenter(std::vector<Point>& points, const int depth) {
    long long sumX = 0;
    long long sumY = 0;
    for (const auto& p : points) {
        sumX += p.x;
        sumY += p.y;
    }
    
    Point mean = {static_cast<int>(sumX / points.size()), static_cast<int>(sumY / points.size())};

    // Find the median based on depth to decide splitting axis
    size_t medianIndex = points.size() / 2;

    if (depth % 2 == axis_first) {
        // Sort by y-coordinate
        std::sort(points.begin(), points.end(), [](const Point& a, const Point& b) {
            return a.y < b.y;
        });

        if (points.size() % 2 == 0) {
            mean.y = (points[medianIndex - 1].y + points[medianIndex].y) / 2;
        } else {
            mean.y = points[medianIndex].y;
        }

        return mean;
    } else {
        // Sort by x-coordinate
        std::sort(points.begin(), points.end(), [](const Point& a, const Point& b) {
            return a.x < b.x;
        });

        if (points.size() % 2 == 0) {
            mean.x = (points[medianIndex - 1].x + points[medianIndex].x) / 2;
        } else {
            mean.x = points[medianIndex].x;
        }
        return mean;
    }
}

// Calculates the mean (average) point of a set of points
Point Tree::calculateMean(const std::vector<Point>& points) {
    long long sumX = 0;
    long long sumY = 0;
    for (const auto& p : points) {
        sumX += p.x;
        sumY += p.y;
    }
    
    Point mean = {static_cast<int>(sumX / points.size()), static_cast<int>(sumY / points.size())};
    return mean;
}

// Calculates the Euclidean distance between two points
double Tree::calculateDistance(const Point& a, const Point& b) {
    return std::sqrt(static_cast<double>((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)));
}

// Recursively builds the tree by splitting points
std::unique_ptr<Node> Tree::buildTreeRecursive(std::vector<Point>& points, int depth) {
    if (points.empty()) {
        return nullptr;
    }

    auto node = std::make_unique<Node>();  // Use custom make_unique

    node->center = calculateMean(points);

    if (points.size() <= 2) {  // max_num_leaf_node = 2
        node->points = points;
        node->isLeaf = true;
        return node;
    }

    // Decide axis based on depth
    int axis = depth % 2;
    if (axis == axis_first) {
        // Sort points by y-coordinate
        std::sort(points.begin(), points.end(), [](const Point& a, const Point& b) {
            return a.y < b.y;
        });
    } else {
        // Sort points by x-coordinate
        std::sort(points.begin(), points.end(), [](const Point& a, const Point& b) {
            return a.x < b.x;
        });
    }

    // Split points at median
    size_t medianIndex = points.size() / 2;
    std::vector<Point> leftPoints(points.begin(), points.begin() + medianIndex);
    std::vector<Point> rightPoints(points.begin() + medianIndex, points.end());

    node->left = buildTreeRecursive(leftPoints, depth + 1);
    node->right = buildTreeRecursive(rightPoints, depth + 1);

    return node;
}

// Adjusts leaf nodes to balance the tree
void Tree::adjustLeafNodes(std::unique_ptr<Node>& node, const int depth) {
    if (!node || node->isLeaf) return;

    if (node->left && node->left->isLeaf && node->right && node->right->isLeaf) {
        // Find distances between points
        double dist1 = 0.0;
        double dist2 = 0.0;
        
        std::vector<Point> allPoints = node->left->points;
        allPoints.insert(allPoints.end(), node->right->points.begin(), node->right->points.end());

        // Sort points by x-coordinate
        std::sort(allPoints.begin(), allPoints.end(), [](const Point& a, const Point& b) {
            return a.x < b.x;
        });
        
        if (allPoints.size() == 3) {
            dist1 = calculateDistance(allPoints[0], allPoints[1]);
            dist2 = calculateDistance(allPoints[1], allPoints[2]);
            if (dist1 > dist2) {
                std::swap(allPoints[0], allPoints[2]);
            }
        }
        
        // Reassign points to left and right nodes to group closer points
        size_t splitIndex = 2;
        node->left->points.assign(allPoints.begin(), allPoints.begin() + splitIndex);
        node->right->points.assign(allPoints.begin() + splitIndex, allPoints.end());

        // Update centers after reassigning points
        node->left->center = calculateMean(node->left->points);
        node->right->center = calculateMean(node->right->points);
    }

    adjustLeafNodes(node->left, depth);
    adjustLeafNodes(node->right, depth);
}

void Tree::rebuildTree(Node* node, std::vector<std::pair<Point, Point>>& errorPaths, const int casetype) {
    if (!node) return;

    for (const auto& path : errorPaths) {
        if (node->center == path.first && node->left->left->isLeaf) {
            std::vector<Point> allPoints; 
            if (casetype == 1) {
                // std::cout << "Rebuilding left node of center (" << node->center.x << ", " << node->center.y << ")\n";
                allPoints = node->left->left->points;
                allPoints.insert(allPoints.end(), node->left->right->points.begin(), node->left->right->points.end());

                // std::sort(allPoints.begin(), allPoints.end(), [](const Point& a, const Point& b) {
                //     return a.x < b.x;
                // });

                // std::cout << "All points size: " << allPoints.size() << std::endl;

                // std::cout << "All points: ";
                // for (const auto& p : allPoints) {
                //     std::cout << "(" << p.x << ", " << p.y << ") ";
                // }

                if (allPoints.size() == 3) {
                    std::swap(allPoints[0], allPoints[2]);
                    node->left->left->points.assign(allPoints.begin(), allPoints.begin() + 2);
                    node->left->right->points.assign(allPoints.begin() + 2, allPoints.end());
                }

                node->left->left->center = calculateMean(node->left->left->points);
                node->left->right->center = calculateMean(node->left->right->points);
            }

            // std::cout << "Rebuilt left node of Point (" <<node->left->left->points[0].x << ", " << node->left->left->points[0].y << "), (" << node->left->left->points[1].x << ", " << node->left->left->points[1].y << ")\n";
            if (casetype == 2) {
                // std::cout << "Rebuilding right node of center (" << node->center.x << ", " << node->center.y << ")\n";
                allPoints = node->right->left->points;
                allPoints.insert(allPoints.end(), node->right->right->points.begin(), node->right->right->points.end());

                // std::sort(allPoints.begin(), allPoints.end(), [](const Point& a, const Point& b) {
                //     return a.x < b.x;
                // });

                if (allPoints.size() == 3) {
                    std::swap(allPoints[0], allPoints[2]);
                    node->right->left->points.assign(allPoints.begin(), allPoints.begin() + 2);
                    node->right->right->points.assign(allPoints.begin() + 2, allPoints.end());
                }

                node->right->left->center = calculateMean(node->right->left->points);
                node->right->right->center = calculateMean(node->right->right->points);
                
                // delet the path in errorPaths
                errorPaths.erase(std::remove(errorPaths.begin(), errorPaths.end(), path), errorPaths.end());
            }

            if (casetype == 3) {
                allPoints = node->left->left->points;
                allPoints.insert(allPoints.end(), node->left->right->points.begin(), node->left->right->points.end());

                // std::sort(allPoints.begin(), allPoints.end(), [](const Point& a, const Point& b) {
                //     return a.x < b.x;
                // });

                // std::cout << "All points size: " << allPoints.size() << std::endl;

                // std::cout << "All points: ";
                // for (const auto& p : allPoints) {
                //     std::cout << "(" << p.x << ", " << p.y << ") ";
                // }

                if (allPoints.size() == 3) {
                    std::swap(allPoints[0], allPoints[2]);
                    node->left->left->points.assign(allPoints.begin(), allPoints.begin() + 2);
                    node->left->right->points.assign(allPoints.begin() + 2, allPoints.end());
                }

                node->left->left->center = calculateMean(node->left->left->points);
                node->left->right->center = calculateMean(node->left->right->points);
            }
        }
    }

    if (node->left) {
        rebuildTree(node->left.get(), errorPaths, casetype);
    }
    if (node->right) {
        rebuildTree(node->right.get(), errorPaths, casetype);
    }
}

// Rebuilds the tree to fix routing issues by adjusting problematic subtrees
// void Tree::rebuildTree(Node* node, std::vector<std::pair<Point, Point>>& errorPaths) {
//     if (!node) return;

//     // Collect paths to remove to avoid modifying the container while iterating
//     std::vector<std::pair<Point, Point>> pathsToRemove;

//     for (const auto& path : errorPaths) {
//         if (node->center == path.second) {
//             Node* childNode = nullptr;

//             // Check if the problematic node is in the left or right subtree
//             if (isNodeInSubtree(node->left.get(), path.first)) {
//                 childNode = node->left.get();
//                 std::cout << "Rebuilding left subtree of center (" << node->center.x << ", " << node->center.y << ")\n";
//             } else if (isNodeInSubtree(node->right.get(), path.first)) {
//                 childNode = node->right.get();
//                 std::cout << "Rebuilding right subtree of center (" << node->center.x << ", " << node->center.y << ")\n";
//             }

//             if (childNode) {
//                 // Collect all leaf nodes under the child node
//                 std::vector<Node*> leafNodes;
//                 collectLeafNodes(childNode, leafNodes);

//                 // Collect all points from leaf nodes
//                 std::vector<Point> allPoints;
//                 for (Node* leaf : leafNodes) {
//                     allPoints.insert(allPoints.end(), leaf->points.begin(), leaf->points.end());
//                 }

//                 // Sort points by x-coordinate (adjust sorting criteria as needed)
//                 std::sort(allPoints.begin(), allPoints.end(), [](const Point& a, const Point& b) {
//                     return a.x < b.x;
//                 });

//                 // Redistribute points based on the total number of points
//                 if (allPoints.size() == 3) {
//                     std::cout << "Redistributing points for 3 points\n";
//                     // Example: assign two points to one leaf node and one point to another
//                     std::vector<Point> group1 = { allPoints[2], allPoints[1] };
//                     std::vector<Point> group2 = { allPoints[0] };

//                     // Reassign points to leaf nodes
//                     if (leafNodes.size() >= 2) {
//                         leafNodes[0]->points = group1;
//                         leafNodes[1]->points = group2;
//                     } else {
//                         // If not enough leaf nodes, log an error
//                         std::cerr << "Error: Not enough leaf nodes to reassign points\n";
//                         continue;
//                     }
//                 }

//                 // Update centers of affected leaf nodes
//                 for (Node* leaf : leafNodes) {
//                     leaf->center = calculateMean(leaf->points);
//                 }

//                 // Mark the path for removal
//                 pathsToRemove.push_back(path);
//             }
//         }
//     }

//     // Remove processed paths from errorPaths
//     for (const auto& path : pathsToRemove) {
//         errorPaths.erase(std::remove(errorPaths.begin(), errorPaths.end(), path), errorPaths.end());
//     }

//     // Recursively process child nodes
//     rebuildTree(node->left.get(), errorPaths);
//     rebuildTree(node->right.get(), errorPaths);
// }

// // Helper function: Checks if a node with the target center exists in the subtree
// bool Tree::isNodeInSubtree(Node* subtree, const Point& targetCenter) {
//     if (!subtree) return false;
//     if (subtree->center == targetCenter) return true;
//     return isNodeInSubtree(subtree->left.get(), targetCenter) || isNodeInSubtree(subtree->right.get(), targetCenter);
// }

// // Helper function: Collects all leaf nodes under a given node
// void Tree::collectLeafNodes(Node* node, std::vector<Node*>& leafNodes) {
//     if (!node) return;
//     if (!node->left && !node->right) {
//         // It's a leaf node
//         leafNodes.push_back(node);
//     } else {
//         collectLeafNodes(node->left.get(), leafNodes);
//         collectLeafNodes(node->right.get(), leafNodes);
//     }
// }

// Prints the tree structure for debugging
void Tree::printTree(Node* node, int depth) {
    if (node == nullptr) return;

    // Indent based on depth
    for (int i = 0; i < depth; ++i) std::cout << "  ";
    std::cout << "Node Center: (" << node->center.x << ", " << node->center.y << ")\n";

    // Print leaf node points
    if (!node->points.empty()) {
        for (int i = 0; i < depth + 1; ++i) std::cout << "  ";
        std::cout << "Leaf Points: ";
        for (const auto& p : node->points) {
            std::cout << "(" << p.x << ", " << p.y << ") ";
        }
        std::cout << "\n";
    }

    // Recursively print left and right subtrees
    printTree(node->left.get(), depth + 1);  // Use .get() to access raw pointer
    printTree(node->right.get(), depth + 1); // Use .get() to access raw pointer
}
