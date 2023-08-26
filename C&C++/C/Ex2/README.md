## Tree Analyzer

This program analyzes a tree structure based on a given graph file and provides various information about the tree's properties.

### Compilation and Execution

To compile and run the Tree Analyzer program, follow these steps:

1. Compile the program using a C compiler:
   ```bash
   $ gcc tree_analyzer.c queue.c -o TreeAnalyzer
   ```
2. Run the compiled executable with the required arguments:
   ```bash
   $ ./TreeAnalyzer <Graph_File_Path> <First_Vertex> <Second_Vertex>
   ```
   Replace <Graph_File_Path>, <First_Vertex>, and <Second_Vertex> with the appropriate values.

### Usage

The program expects the following command-line arguments:

- `Graph_File_Path`: The path to the file containing the graph data.
- `First_Vertex`: The first vertex for path analysis.
- `Second_Vertex`: The second vertex for path analysis.

The program reads the graph data from the specified file, analyzes the tree structure, and provides the following information:

- Root Vertex: The root vertex of the tree.
- Vertices Count: The total number of vertices in the tree.
- Edges Count: The total number of edges in the tree.
- Length of Minimal Branch: The length of the shortest path from the root to a leaf.
- Length of Maximal Branch: The length of the longest path from the root to a leaf.
- Diameter Length: The length of the longest path between any two nodes in the tree.
- Shortest Path Between <First_Vertex> and <Second_Vertex>: The shortest path between the specified vertices.

### Data Structures

The program uses the following data structures:

- `Node`: Represents a node in the tree.
- `Tree:` Represents the entire tree structure.

### Algorithms

The program utilizes the Breadth-First Search (BFS) algorithm to analyze the tree and find various properties such as root, branch lengths, and shortest paths.

### Input Format

The graph data file should have the following format:

- The first line contains the number of vertices in the tree.
- Each subsequent line represents a node and its adjacent nodes.

Nodes are represented by numbers, and adjacency is indicated by space-separated numbers.

### Output Format

The program outputs information about the tree structure and properties in a readable format.

### Error Handling

The program performs error checks for invalid input data and file operations. If errors are encountered, appropriate error messages are displayed.

### Example

Suppose you have a graph file `tree.txt` with the following content:

6
0 1
1 2 3
2
3 4
4 5

You can analyze the tree as follows:
```bash
$ ./TreeAnalyzer tree.txt 2 5
```

The program will display information about the tree structure and properties based on the provided graph data and vertices.

### Note

```perl
Please replace the placeholders (`<Graph_File_Path>`, `<First_Vertex>`, and `<Second_Vertex>`) with actual values when using the program. Also, note that the example graph data provided in the documentation (`tree.txt`) is for illustration purposes; you should use your own graph data when running the program.
```
