# Clock Tree Synthesis (CTS) Program

This program performs clock tree synthesis (CTS) using Lee's algorithm for pathfinding. It reads an input file containing clock source and sink coordinates, builds a clock tree, and routes wires while maintaining a specified gap from obstacles. The program outputs the routed clock tree as a set of line segments and provides metrics like maximum and minimum arrival times, skew ratio, and total wire length.

---

## Table of Contents

- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
  - [Command-Line Arguments](#command-line-arguments)
  - [Input File Format](#input-file-format)
  - [Output File Format](#output-file-format)
- [Compilation and Execution](#compilation-and-execution)
- [Example](#example)
- [Algorithm Details](#algorithm-details)
  - [Tree Building](#tree-building)
  - [Pathfinding with Gap Constraint](#pathfinding-with-gap-constraint)
  - [Line Segment Extraction](#line-segment-extraction)
- [Files and Directories](#files-and-directories)
- [License](#license)

---

## Features

- **Clock Tree Construction**: Builds a clock tree from given source and sink coordinates.
- **Lee's Algorithm**: Uses Lee's algorithm for pathfinding to ensure the shortest path is found.
- **Gap Constraint**: Maintains a specified gap between wires and obstacles during routing.
- **Directional Preferences**: During backtracking, the algorithm can prefer vertical or horizontal paths.
- **No Cycles**: Ensures that the routing paths do not contain cycles.
- **Metrics Calculation**: Provides maximum and minimum arrival times, skew ratio, and total wire length.

---

## Requirements

- **C++ Compiler**: Compatible with C++11 or later.
- **Make**: For building the project using the provided `Makefile`.

---

## Installation

Clone the repository or download the source code files into a directory.

---

## Usage

### Command-Line Arguments

```
Usage: cts input.cts output.cts
```

- `input.cts`: The input file containing clock source and sink coordinates.
- `output.cts`: The output file where the routed clock tree will be saved.

### Input File Format

The input file should have the following format:

```
.p <number_of_pins>   # number of pins
.dimx <dimX> # chip dimension in x-axis
.dimy <dimY> # chip dimension in y-axis
<source_x> <source_y>  # clock source coordinates
<sink1_x> <sink1_y>  # #1 clock sink coordinates
<sink2_x> <sink2_y>  # #2 clock sink coordinates
...
.e
```

- `.p`: Indicates the number of pins (sinks + 1 source).
- `.dimx` and `.dimy`: Dimensions of the grid (chip).
- Coordinates: List of coordinates for the source and sinks.
- `.e`: End of the input file.

**Example:**

```
.p 5   # number of pins 
.dimx 110 # chip dimension in x-axis 
.dimy 100 # chip dimension in y-axis 
97 38  # clock source coordinates 
49 62  # #1 clock sink coordinates 
17 25  # #2 clock sink coordinates 
56 82  # #3 clock sink coordinates 
72 27  # #4 clock sink coordinates 
.e
```

### Output File Format

The output file contains the routed clock tree represented as line segments:

```
.l <number_of_line_segments>   # number of line segments
.dimx <dimX> # chip dimension in x-axis
.dimy <dimY> # chip dimension in y-axis
<start_x1> <start_y1> <end_x1> <end_y1>
<start_x2> <start_y2> <end_x2> <end_y2>
...
.e
```

- `.l`: Indicates the number of line segments.
- `.dimx` and `.dimy`: Dimensions of the grid (chip).
- Line segments: Each line represents a line segment from `(start_x, start_y)` to `(end_x, end_y)`.
- `.e`: End of the output file.

---

## Compilation and Execution

### Compile the Program

Ensure you have a C++ compiler and `make` installed. Open a terminal in the directory containing the source files and run:

```bash
make
```

This will compile the program and create an executable named `cts`.

### Run the Program

Execute the program using:

```bash
./cts input.cts output.cts
```

- Replace `input.cts` with your input file.
- Replace `output.cts` with the desired output file name.

**Examples:**

- Without gap:

  ```bash
  ./cts input.cts output.cts
  ```
---

## Example

Given the following `input.cts` file:

```
.p 5   # number of pins 
.dimx 110 # chip dimension in x-axis 
.dimy 100 # chip dimension in y-axis 
97 38  # clock source coordinates 
49 62  # #1 clock sink coordinates 
17 25  # #2 clock sink coordinates 
56 82  # #3 clock sink coordinates 
72 27  # #4 clock sink coordinates 
.e
```

Run the program:

```bash
./cts --gap 2 input.cts output.cts
```

**Sample Console Output:**

```
T_max: 203, T_min: 36, Skew ratio: 5.63889
W_cts: 203
```

**Sample `output.cts` Content:**

```
.l 8   # number of line segments
.dimx 110 # chip dimension in x-axis
.dimy 100 # chip dimension in y-axis
17 25 17 62
17 62 49 62
72 82 72 27
72 27 97 27
49 62 49 82
49 82 56 82
56 82 72 82
97 27 97 38
.e
```

**Visualization:**

Use a visualization tool or script to plot the line segments from `output.cts` to see the routed clock tree.

---

## Algorithm Details

### Tree Building

- **Purpose**: Organize the sinks into a binary tree to determine the routing order.
- **Method**: Uses a simple clustering algorithm to build a balanced binary tree.
- **Implementation**: Defined in `Tree.h` and `Tree.cpp`.

### Pathfinding with Gap Constraint

- **Algorithm**: Lee's algorithm (a BFS-based pathfinding algorithm).
- **Gap Functionality**:
  - **Gap Check**: During BFS traversal, before moving to the next cell, the algorithm looks ahead up to `gap` cells in the intended direction.
  - **Obstacle Avoidance**: If an obstacle or wire is found within the gap distance, that direction is blocked.
  - **Relaxed Gap Near End Point**: If the Euclidean distance to the end point is less than or equal to `gap`, the gap check is relaxed.
- **Directional Preferences**:
  - **Backtracking from Target to Source**: Prefers vertical moves to shape the path.
  - **Backtracking from Source to Target**: Prefers horizontal moves.
- **Implementation**: Defined in `BFS.h` and `BFS.cpp`.

### Line Segment Extraction

- **Purpose**: After all paths are marked on the grid, extract the continuous wires as line segments.
- **Method**:
  - **Marking**: Paths are marked with `WIRE` and turn points with `TURN_POINT` during routing.
  - **Scanning**: The grid is scanned first horizontally, then vertically, to collect segments.
  - **Segment Formation**: Continuous `WIRE`s are combined into single line segments, starting and ending one cell beyond the wire sequence.
- **Implementation**: Defined in `main.cpp` (`markPathOnGrid` and `extractSegments` functions).

---

## Files and Directories

- **Source Files**:
  - `main.cpp`: The main program file containing the execution flow.
  - `Point.h` / `Point.cpp`: Defines the `Point` class representing coordinates.
  - `Node.h` / `Node.cpp`: Defines the `Node` class for tree nodes.
  - `Tree.h` / `Tree.cpp`: Handles tree construction.
  - `Grid.h` / `Grid.cpp`: Represents the grid and provides utility functions.
  - `BFS.h` / `BFS.cpp`: Implements the BFS pathfinding algorithm with gap functionality.
  - `LineSegment.h` / `LineSegment.cpp`: Defines the `LineSegment` class.
  - `Constants.h`: Contains constant definitions used across the program.

- **Makefile**: Automates the build process.

---

## License

This project is released under the MIT License.

---

**Note**: For any issues or questions, please contact the project maintainer.
