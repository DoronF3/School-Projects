## Red-Black Tree Implementation (`RBTree.c`)

The `RBTree.c` file contains the implementation of a Red-Black Tree data structure. Red-Black Trees are a type of self-balancing binary search tree that ensures efficient insertion, deletion, and retrieval operations.

### Introduction

The Red-Black Tree is a binary search tree with additional properties to ensure that it remains balanced. These properties include coloring each node as red or black and performing rotations and recoloring operations during insertions and deletions to maintain balance.

### Functions

- `RBTree *newRBTree(CompareFunc compFunc, FreeFunc freeFunc)`: Constructs a new Red-Black Tree with the given comparison and free functions.
- `void leftRotation(Node *node, RBTree *tree)`: Performs a left rotation on the given node in the Red-Black Tree.
- `void rightRotation(Node *node, RBTree *tree)`: Performs a right rotation on the given node in the Red-Black Tree.
- `void balance(char flag, Node *parent, Node *grand, RBTree *tree)`: Balances the Red-Black Tree based on the given positions and relationships of nodes.
- `void fix(CompareFunc func, Node *node, Node *parent, char flag, RBTree *tree)`: Checks and fixes the state of the tree based on various situations during insertion.
- `int addToRBTree(RBTree *tree, void *data)`: Adds an item to the Red-Black Tree.
- `int containsRBTree(RBTree *tree, void *data)`: Checks if the Red-Black Tree contains a specific item.
- `int forEachRBTree(RBTree *tree, forEachFunc func, void *args)`: Activates a function on each item in the Red-Black Tree in ascending order.
- `void freeNode(struct Node *node, FreeFunc func)`: Frees memory allocated for a node and its children in the Red-Black Tree.
- `void freeRBTree(RBTree *tree)`: Frees memory associated with the entire Red-Black Tree.

### Usage

```c
// Example usage of RBTree functions
RBTree *tree = newRBTree(stringCompare, freeString);
addToRBTree(tree, "apple");
addToRBTree(tree, "banana");
int containsApple = containsRBTree(tree, "apple");  // Should return 1
int containsCherry = containsRBTree(tree, "cherry"); // Should return 0
freeRBTree(tree);
```

## Vector and String Structures and Functions (`Structs.c`)

The `Structs.c` file contains the implementation of structures and functions related to vectors and strings used in conjunction with the Red-Black Tree.

### Introduction

This file implements structures and functions necessary for working with vectors and strings. These are used in tandem with the Red-Black Tree implementation to provide more complex data handling capabilities.

### Functions

- `int stringCompare(const void *a, const void *b)`: Compares two strings lexicographically for the Red-Black Tree's comparison function.
- `int concatenate(const void *word, void *pConcatenated)`: Concatenates a word to an existing string in memory.
- `void freeString(void *s)`: Frees memory allocated for a string.
- `int vectorCompare1By1(const void *a, const void *b)`: Compares vectors element by element for Red-Black Tree's comparison function.
- `void freeVector(void *pVector)`: Frees memory allocated for a vector.
- `double calcNorm(const void *pVector)`: Calculates the power of the norm of a vector.
- `int copyIfNormIsLarger(const void *pVector, void *pMaxVector)`: Copies a vector if its norm is larger than the norm of another vector.
- `Vector *findMaxNormVectorInTree(RBTree *tree)`: Finds the vector with the largest norm in the Red-Black Tree.

### Usage

```c
// Example usage of Vector functions
Vector *vector1 = createVector(3, (double[]){1.0, 2.0, 3.0});
Vector *vector2 = createVector(3, (double[]){2.0, 3.0, 4.0});
int comparisonResult = vectorCompare1By1(vector1, vector2);  // Should return -1
double norm = calcNorm(vector1);
Vector *maxNormVector = findMaxNormVectorInTree(tree);
freeVector(vector1);
freeVector(vector2);
freeVector(maxNormVector);
```
