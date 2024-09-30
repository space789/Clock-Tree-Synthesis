///////////////////////////
// Main program for CTS  //
// Author: Shiina        //
// Date: 2024/09/30      //
// Version: 1.0          //
// copiright 2024        //
///////////////////////////

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <map>
#include <algorithm>
#include <queue>

#include "Point.h"
#include "Node.h"
#include "Tree.h"
#include "Grid.h"
#include "BFS.h"
#include "LineSegment.h"
#include "Constants.h"

void readInputFile(const std::string& filename, int& numPins, int& dimX, int& dimY,
                   Point& source, std::vector<Point>& sinks) {
    std::ifstream infile(filename);
    std::string line;
    bool readingPins = false;

    while (std::getline(infile, line)) {
        if (line.empty()) continue;
        if (line[0] == '.') {
            if (line[1] == 'p') {
                numPins = std::stoi(line.substr(3));
            } else if (line.substr(0, 5) == ".dimx") {
                dimX = std::stoi(line.substr(6));
            } else if (line.substr(0, 5) == ".dimy") {
                dimY = std::stoi(line.substr(6));
            } else if (line.substr(0, 2) == ".e") {
                break;
            }
        } else {
            std::istringstream iss(line);
            int x, y;
            iss >> x >> y;
            if (!readingPins) {
                source = Point(x, y);
                readingPins = true;
            } else {
                sinks.push_back(Point(x, y));
            }
        }
    }
}

void writeOutputFile(const std::string& filename, const std::vector<LineSegment>& segments,
                     int dimX, int dimY) {
    std::ofstream outfile(filename);
    // outfile << ".l " << segments.size() << "   # number of line segments\n";
    // outfile << ".dimx " << dimX << " # chip dimension in x-axis\n";
    // outfile << ".dimy " << dimY << " # chip dimension in y-axis\n";
    outfile << ".l " << segments.size() << std::endl;
    outfile << ".dimx " << dimX << std::endl;
    outfile << ".dimy " << dimY << std::endl;
    for (const auto& seg : segments) {
        outfile << seg.start.x << " " << seg.start.y << " "
                << seg.end.x << " " << seg.end.y << "\n";
    }
    outfile << ".e\n";
}

void markPathOnGrid(Grid& grid, const std::vector<Point>& path, std::shared_ptr<std::vector<LineSegment>>& segments) {
    Point prev = path[0];
    Point start = path[0];
    Point end = path[path.size() - 1];
    if (grid.getValue(prev) == EMPTY)
        grid.markCell(prev, WIRE);

    for (size_t i = 1; i < path.size(); ++i) {
        Point current = path[i];
        grid.markCell(current, WIRE);

        // Check for turn point
        int dirX = current.x - prev.x;
        int dirY = current.y - prev.y;

        if (i > 1) {
            Point prev2 = path[i - 2];
            int prevDirX = prev.x - prev2.x;
            int prevDirY = prev.y - prev2.y;

            if ((dirX != prevDirX) || (dirY != prevDirY)) {
                // Turn detected at prev
                grid.markCell(prev, TURN_POINT);
                segments->emplace_back(start, prev);
                start = prev;
            }
        }
        prev = current;
    }

    segments->emplace_back(start, end);
}


void getTreeLevels(Node* node, int depth, std::map<int, std::vector<Node*>>& levels) {
    if (!node) return;
    levels[depth].push_back(node);
    if (node->left) getTreeLevels(node->left.get(), depth + 1, levels);
    if (node->right) getTreeLevels(node->right.get(), depth + 1, levels);
}

void connectNodesAtLevel(Grid& grid, const std::vector<Node*>& nodes, const int depth, std::map<Point, int>& arrivalTimes, std::shared_ptr<std::vector<LineSegment>>& segments, std::vector<std::pair<Point, Point>>& errorPaths) {
    bool vorh = depth % 2 == 0; // Vertical or horizontal
    for (auto node : nodes) {
        // Mark center of the node
        // grid.markCell(node->center, CENTER);

        if (node->isLeaf) {
            // Connect each point to the node's center
            for (const auto& p : node->points) {
                if (node->points.size() == 1) continue; // Skip if single point
                auto path = BFS::leeAlgorithm(grid, p, node->center, vorh);
                if (path.empty()) {
                    std::cout << "No path found from leaf node (" << p.x << ", " << p.y << ") to center (" << node->center.x << ", " << node->center.y << ")" << std::endl;
                    continue; // No path found
                }
                grid.reset();
                markPathOnGrid(grid, path, segments);
                // arrivalTimes[p] = arrivalTimes[node->center] + path.size() - 1;
                arrivalTimes[p] = path.size() - 1;
            }
        } else {
            // Connect children's centers to this node's center
            if (node->left) {
                auto path = BFS::leeAlgorithm(grid, node->left->center, node->center, vorh);
                if (path.empty()) {
                    std::cout << "No path found from left leaf node (" << node->left->center.x << ", " << node->left->center.y << ") to center (" << node->center.x << ", " << node->center.y << ")" << std::endl;
                    errorPaths.push_back({node->left->center, node->center});
                    continue; // No path found   
                }
                grid.reset();
                markPathOnGrid(grid, path, segments);
                // arrivalTimes[node->left->center] = arrivalTimes[node->center] + path.size() - 1;
                arrivalTimes[node->left->center] = path.size() - 1;
            }
            if (node->right) {
                auto path = BFS::leeAlgorithm(grid, node->right->center, node->center, vorh);
                if (path.empty()) {
                    std::cout << "No path found from right leaf node (" << node->right->center.x << ", " << node->right->center.y << ") to center (" << node->center.x << ", " << node->center.y << ")" << std::endl;
                    errorPaths.push_back({node->right->center, node->center});
                    continue; // No path found   
                }
                grid.reset();
                markPathOnGrid(grid, path, segments);
                // arrivalTimes[node->right->center] = arrivalTimes[node->center] + path.size() - 1;
                arrivalTimes[node->right->center] = path.size() - 1;
            }
        }
    }
}

// FIXME: This function is not correct
void calculateArrivalTimes(Node* node, std::map<Point, int>& arrivalTimes, int accumulatedDistance = 0) {
    if (!node) return;

    // 累加當前節點的 center 到目前的 accumulatedDistance
    int currentDistance = accumulatedDistance + arrivalTimes[node->center];

    // 如果當前節點是 leaf node，計算每個點的 arrival time
    if (node->isLeaf) {
        for (const auto& p : node->points) {
            // std::cout << "Arrival time to leaf node (" << p.x << ", " << p.y << "): to center (" << node->center.x << ", " << node->center.y << ") = " << arrivalTimes[p] << std::endl; 
            arrivalTimes[p] = currentDistance + arrivalTimes[p];
            // arrivalTimes[p] = currentDistance;
        }
    }

    // 遞迴處理 leaf 節點
    if (node->left) {
        calculateArrivalTimes(node->left.get(), arrivalTimes, currentDistance);
    }

    // 遞迴處理 right 節點
    if (node->right) {
        calculateArrivalTimes(node->right.get(), arrivalTimes, currentDistance);
    }
}



void marktreecenter(Grid& grid, Node* root) {
    if (!root) return;
    if (grid.getValue(root->center) == EMPTY)
        grid.markCell(root->center, CENTER);
    if (root->left) marktreecenter(grid, root->left.get());
    if (root->right) marktreecenter(grid, root->right.get());
}

void connectTree(Grid& grid, Node* root, std::map<Point, int>& arrivalTimes, const Point& source, std::shared_ptr<std::vector<LineSegment>>& segments, std::vector<std::pair<Point, Point>>& errorPaths) {
    std::map<int, std::vector<Node*>> levels;
    getTreeLevels(root, 0, levels);

    int maxDepth = levels.rbegin()->first;
    // std::cout << "Max depth: " << maxDepth << std::endl;
    for (int depth = maxDepth; depth >= 0; --depth) {
        connectNodesAtLevel(grid, levels[depth], depth, arrivalTimes, segments, errorPaths);
    }
    // for (int depth = 0; depth <= maxDepth; ++depth) {
    //     connectNodesAtLevel(grid, levels[depth], arrivalTimes, segments);
    // }
}


int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: cts [--gap <int>] input.cts output.cts\n";
        return 1;
    }

    int gap = 0; // Default gap
    std::string inputFile, outputFile;

    int argIndex = 1;
    while (argIndex < argc - 2) {
        std::string arg = argv[argIndex];
        if (arg == "--gap") {
            gap = std::stoi(argv[++argIndex]);
        } else {
            std::cout << "Unknown option: " << arg << "\n";
            return 1;
        }
        ++argIndex;
    }

    inputFile = argv[argIndex++];
    outputFile = argv[argIndex];

    int numPins = 0, dimX = 0, dimY = 0;
    Point source;
    std::vector<Point> sinks;
    readInputFile(inputFile, numPins, dimX, dimY, source, sinks);

    // Initialize segments
    auto segments = std::make_shared<std::vector<LineSegment>>();

    // Build tree
    std::vector<Point> allPoints = sinks;
    Tree tree;
    tree.buildTree(allPoints);

    // Print tree
    tree.printTree(tree.root.get(), 0);


    // Initialize grid
    Grid grid(dimX, dimY, gap);
    grid.markCell(source, SOURCE);
    for (const auto& sink : sinks) {
        grid.markCell(sink, OBSTACLE); // Sinks are obstacles during routing
    }

    // Mark tree centers
    // marktreecenter(grid, tree.root.get());

    // Initialize error path (start, end)
    std::vector<std::pair<Point, Point>> errorPaths = {};

    // Wiring
    std::map<Point, int> arrivalTimes;
    arrivalTimes[tree.root->center] = 0;

    // Connect tree nodes from bottom to top
    connectTree(grid, tree.root.get(), arrivalTimes, source, segments, errorPaths);

    // If error paths exist, try to rebuild tree
    for (int i = 1; i < 4; i++) {
        if (!errorPaths.empty()) {
            std::cout << "Rebuilding tree...\n";
            segments->clear();
            arrivalTimes.clear();
            arrivalTimes[tree.root->center] = 0;
            tree.rebuildTree(tree.root.get(), errorPaths, i);
            grid.resetall();
            grid.markCell(source, SOURCE);
            for (const auto& sink : sinks) {
                grid.markCell(sink, OBSTACLE); // Sinks are obstacles during routing
            }
            // marktreecenter(grid, tree.root.get());
            errorPaths.clear();
            connectTree(grid, tree.root.get(), arrivalTimes, source, segments, errorPaths);
        } else {
            std::cout << "No error paths found. Tree is successfully routed.\n";
            break;
        }
    }

    // Connect source to root center
    auto path = BFS::leeAlgorithm(grid, source, tree.root->center, false);
    if (!path.empty()) {
        grid.reset();
        markPathOnGrid(grid, path, segments);
        arrivalTimes[tree.root->center] = path.size() - 1;
        // std::cout << "Arrival time to root center: " << arrivalTimes[tree.root->center] << "\n";
    } else {
        std::cerr << "No path found from source to root center.\n";
    }

    grid.reset();

    calculateArrivalTimes(tree.root.get(), arrivalTimes);

    // Calculate arrival times to sinks
    std::vector<int> sinkArrivalTimes;
    for (const auto& sink : sinks) {
        if (arrivalTimes.find(sink) != arrivalTimes.end()) {
            sinkArrivalTimes.push_back(arrivalTimes[sink]);
            // std::cout << "Arrival time to sink at (" << sink.x << ", " << sink.y << "): " << arrivalTimes[sink] << "\n";
        } else {
            std::cerr << "No path found to sink at (" << sink.x << ", " << sink.y << ").\n";
        }
    }

    // Compute T_max, T_min, skew ratio, total wire length
    int Tmax = *std::max_element(sinkArrivalTimes.begin(), sinkArrivalTimes.end());
    int Tmin = *std::min_element(sinkArrivalTimes.begin(), sinkArrivalTimes.end());
    double skewRatio = static_cast<double>(Tmax) / Tmin;
    int totalWireLength = 0;
    for (const auto& seg : *segments) {
        totalWireLength += seg.length();
    }

    // Write output file
    writeOutputFile(outputFile, *segments, dimX, dimY);

    // Output metrics
    std::cout << "T_max: " << Tmax << ", T_min: " << Tmin << ", Skew ratio: " << skewRatio << "\n";
    std::cout << "W_cts: " << totalWireLength << "\n";

    return 0;
}
