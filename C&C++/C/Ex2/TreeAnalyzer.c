#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "queue.h"

#define NUM_ARGS 4
#define FALSE 1
#define TRUE 0
#define MAX_LINE 1024
#define USAGE_MSG "Usage: TreeAnalyzer <Graph File Path> <First Vertex> <Second Vertex>\r\n"
#define INPUT_MSG "Invalid input\r\n"
#define READ "r"
#define NO_SONS 45
#define SPACE 32
#define MEMORY_MSG "A problem with memory allocation occur.\r\n"
#define MEMORY_PROB -2
#define ROOT_MSG "Root Vertex: "
#define VERTI_MSG "Vertices Count: "
#define EDGES_MSG "Edges Count: "
#define MIN_LEN_MSG "Length of Minimal Branch: "
#define MAX_LEN_MSG "Length of Maximal Branch: "
#define DIAM_MSG "Diameter Length: "
#define PATH_MSG "Shortest Path Between "
#define REGEX "%s%d\r\n"
#define DEFAULT_VAL -1


/**
 * A struct that represents a node.
 */
typedef struct Node
{
    int key;
    int parent;
    int isLeaf;
    int *next;
    int distance;
    int numSons;
    int prev;
}
        Node;

/**
 * A struct that represents a tree.
 */
typedef struct Tree
{
    int root;
    int vertices;
    int maxDKEy;
}
        Tree;

/**
 * A method that checks that the given array includes only valid characters.
 * @param charArr An array of characters.
 * @param vertices An int representing the number of the vertices in the tree.
 * @param iter An int representing the current line aka node key.
 * @param treeArr An array of nodes.
 * @return 1 if the array contains invalid characters, 0 otherwise.
 */
int checkSons(const char *charArr, const int vertices, int iter, Node **treeArr)
{
    if (iter > vertices + 1) // Check that no extra lines were given.
    {
        return FALSE;
    }
    if (iter == vertices && strlen(charArr) != 0)
        // Check if the the extra given line is empty.
    {
        return FALSE;
    }
    else if (iter == vertices)
    {
        return TRUE;
    }
    if (strlen(charArr) == 0 || (strlen(charArr) == 1 && (charArr[0] != NO_SONS && charArr[0] < 48 && charArr[0] > 57)))
        // Check that if one char was given it is valid.
    {
        return FALSE;
    }
    else if (strlen(charArr) == 1 && charArr[0] == NO_SONS) // No sons
    {
        return TRUE;
    }
    int digit = 0, spaces = 0, nodes = 0;
    for (int i = 0; i < (int) strlen(charArr); i++)
    {
        if (charArr[i] >= 48 && charArr[i] <= 57) // Char is a number.
        {
            digit++; // Got a number increment counter.
            if (i == ((int) strlen(charArr) - 1) && spaces < 2)
            {
                int numDig = i - digit + 1;
                char *num = (char *) malloc((digit + 1) * sizeof(char)); // Create an array for the given number length.
                if (num == NULL)
                {
                    return MEMORY_PROB;
                }
                num[digit] = '\0';
                for (int c = 0; c < digit; c++)
                {
                    num[c] = charArr[numDig]; // Add the chars to the new array.
                    numDig++;
                }
                int verti = (int) strtol(num, NULL, 10);
                if (verti >= vertices || nodes == vertices) // Check for a legal number.
                {
                    free(num);
                    num = NULL;
                    return FALSE;
                }
                (*treeArr)[iter].numSons++;
                (*treeArr)[iter].next = (int *) realloc((*treeArr)[iter].next, (*treeArr)[iter].numSons * sizeof(int));
                if ((*treeArr)[iter].next == NULL)
                {
                    free(num);
                    num = NULL;
                    return MEMORY_PROB;
                }
                (*treeArr)[iter].next[(*treeArr)[iter].numSons - 1] = verti; // Add the son key to the array.
                if ((*treeArr)[verti].parent != -1) // Check that the node has no father.
                {
                    free(num);
                    num = NULL;
                    return FALSE;
                }
                (*treeArr)[verti].parent = (*treeArr)[iter].key;
                (*treeArr)[iter].isLeaf = 1;
                free(num);
                num = NULL;
                nodes++;
                return TRUE;
            }
        }
        else if (charArr[i] == SPACE && digit != 0 && spaces < 2) // Got space.
        {
            spaces++;
            int numDig = i - digit - spaces + 1; // the number of digits we got.
            char *num = malloc((digit + 1) * sizeof(char)); // Create an array for the given number length.
            if (num == NULL)
            {
                return MEMORY_PROB;
            }
            num[digit] = '\0';
            for (int c = 0; c < digit; c++)
            {
                num[c] = charArr[numDig]; // Add the chars to the new array.
                numDig++;
            }
            int verti = (int) strtol(num, NULL, 10);
            if (verti >= vertices || nodes == vertices) // Check for a legal number.
            {
                free(num);
                num = NULL;
                return FALSE;
            }
            (*treeArr)[iter].numSons++;
            (*treeArr)[iter].next = (int *) realloc((*treeArr)[iter].next, (*treeArr)[iter].numSons * sizeof(int));
            // Resize based on number of sons.
            if ((*treeArr)[iter].next == NULL)
            {
                free(num);
                num = NULL;
                return MEMORY_PROB;
            }
            (*treeArr)[iter].next[(*treeArr)[iter].numSons - 1] = verti; // Add the son key to the array.
            if ((*treeArr)[verti].parent != -1) // Check that the node has no father.
            {
                free(num);
                num = NULL;
                return FALSE;
            }
            (*treeArr)[verti].parent = (*treeArr)[iter].key;
            (*treeArr)[iter].isLeaf = 1;
            digit = 0;
            spaces = 0;
            free(num);
            num = NULL;
            nodes++;
        }
        else if (charArr[i] == SPACE && digit != 0)// Got more than 2 spaces and a digit.
        {
            continue;
        }
        else if (charArr[i] != SPACE)
        {
            return FALSE;
        }
    }
    return TRUE;
}

/**
 * A method that checks that the given array includes only valid characters.
 * @param num An array of characters.
 * @return -1 if the array contains invalid characters, The number of vertices otherwise.
 */
int checkFirstLine(const char *num)
{
    if (num[0] <= 48 || num[0] > 57) // The first char is 0 or not a number.
    {
        return DEFAULT_VAL;
    }
    for (int i = 0; i < (int) strlen(num); i++)
    {
        if (num[i] < 48 || num[i] > 57) // The current char is not a number.
        {
            return DEFAULT_VAL;
        }
    }
    return (int) strtol(num, NULL, 10);
}

/**
 * A method that initializes all the nodes on the given node array.
 * @param treeArr The given node array.
 * @param vertices The number of given vertices.
 */
int initialize(Node **treeArr, int vertices)
{
    for (int i = 0; i < vertices; i++)
    {
        Node node;
        node.key = i;
        node.next = (int *) malloc(sizeof(int));
        if (node.next == NULL) // Check that the malloc operation succeeded.
        {
            return MEMORY_PROB;
        }
        node.next[0] = DEFAULT_VAL;
        node.parent = DEFAULT_VAL;
        node.isLeaf = 0;
        node.distance = DEFAULT_VAL;
        node.numSons = 0;
        node.prev = DEFAULT_VAL;
        (*treeArr)[i] = node;
    }
    return TRUE;
}

/**
 * A method that frees all the memory occupied by the nodes.
 * @param treeArr The given node array.
 * @param vertices The number of given vertices.
 */
void freeAll(Node **treeArr, int vertices)
{
    for (int i = 0; i < vertices; i++)
    {
        free((*treeArr)[i].next);
        (*treeArr)[i].next = NULL;
    }
    free((*treeArr));
    (*treeArr) = NULL;
}

/**
 * A method that checks that the given cli nodes are valid.
 * @param charNode A pointer ot the current char array.
 * @param vertices The number of nodes in the tree.
 * @return 1 if even one of the nodes is invalid, 0 otherwise.
 */
int checkValidNodes(char ***charNode, int vertices)
{
    for (int j = 2; j < 4; j++)
    {
        for (int i = 0; i < (int) strlen((*charNode)[j]); i++)
        {
            if ((*charNode)[j][i] < 48 || (*charNode)[j][i] > 57) // The current char is not a number.
            {
                return FALSE;
            }
        }
        if ((int) strtol((*charNode)[j], NULL, 10) >= vertices)
        {
            return FALSE;
        }
    }
    return TRUE;
}

/**
 * A method that finds the root in the tree.
 * @param treeArr An array of all the nodes of the tree.
 * @param vertices The number of vertices on the tre.
 * @return The key of the root.
 */
int findRoot(Node **treeArr, const int vertices)
{
    for (int i = 0; i < vertices; i++)
    {
        if ((*treeArr)[i].parent == -1) // No father.
        {
            return (*treeArr)[i].key; // Return its key.
        }
    }
    return FALSE;
}

/**
 * A method that implements BFS algorithm.
 * @param treeArr An array of all the nodes of the tree.
 * @param key A key of a node.
 * @param vertices The number of nodes in the tree.
 */
void bfs(Node **treeArr, const int key, const int vertices)
{
    for (int i = 0; i < vertices; i++)
    {
        (*treeArr)[i].distance = DEFAULT_VAL;
    }
    (*treeArr)[key].distance = 0;
    if ((*treeArr)[key].isLeaf == FALSE)
    {
        (*treeArr)[key].prev = DEFAULT_VAL;
    }
    Queue *q = allocQueue();
    enqueue(q, key);
    while (queueIsEmpty(q) != 1)
    {
        unsigned int u = dequeue(q);
        for (int j = 0; j < (*treeArr)[u].numSons; j++)
        {
            int nodeKey = (*treeArr)[u].next[j];
            if ((*treeArr)[nodeKey].distance == DEFAULT_VAL)
            {
                enqueue(q, nodeKey);
                (*treeArr)[nodeKey].prev = (int) u;
                (*treeArr)[nodeKey].distance = ((*treeArr)[u].distance + 1);
            }
        }
        if ((*treeArr)[u].parent != DEFAULT_VAL)
        {
            if ((*treeArr)[(*treeArr)[u].parent].distance == DEFAULT_VAL)
            {
                enqueue(q, (*treeArr)[u].parent);
                (*treeArr)[(*treeArr)[u].parent].prev = (int) u;
                (*treeArr)[(*treeArr)[u].parent].distance = ((*treeArr)[u].distance + 1);
            }
        }
    }
    freeQueue(&q);
    q = NULL;
}

/**
 * A method that returns the maximal or minimal value of the branch in the tree.
 * @param treeArr The array of nodes.
 * @param vertices The number of nodes in the tree.
 * @param tree A struct tree object.
 * @param minmax The value to dictate if we are looking for max or min.
 * @return The maximum distance if minmax is 0, otherwise the minimal distance.
 */
int getHeight(Node **treeArr, const int vertices, Tree *tree, const int minmax)
{
    int dist = 0;
    if (vertices == 1)
    {
        (*tree).maxDKEy = 0;
        return dist;

    }
    if (minmax == 1 && (*tree).vertices == 1)
    {
        dist = (*treeArr)[(*tree).root].distance;
        return dist;
    }
    for (int i = 0; i < vertices; i++)
    {
        if (minmax == 0) // Need to find maximum.
        {
            if ((*treeArr)[i].distance > dist)
            {
                dist = (*treeArr)[i].distance;
                (*tree).maxDKEy = i;
            }
        }
        else if (minmax == 1 && (*treeArr)[i].isLeaf == TRUE) // Need to find minimum, set the first distance.
        {
            if (dist == 0)
            {
                dist = (*treeArr)[i].distance;
            }
            else if ((*treeArr)[i].distance < dist)
            {
                dist = (*treeArr)[i].distance;
            }
        }
    }
    return dist;
}

/**
 * A method that prints the shortest path between two given nodes.
 * @param treeArr A pointer to the array of the nodes.
 * @param nodeStart The start of the path.
 * @param nodeEnd The end of the path.
 */
void printPath(Node **treeArr, const int nodeStart, const int nodeEnd)
{
    if (nodeStart == nodeEnd)
    {
        printf("%d\r\n", nodeEnd);
    }
    else
    {
        int prev = nodeStart;
        while (prev != nodeEnd)
        {
            printf("%d%s", prev, " ");
            prev = (*treeArr)[prev].prev;
        }
        printf("%d\r\n", nodeEnd);
    }
}

/**
 * The main method that runs the program.
 * @param argc An int representing The amount of arguments given through the command line.
 * @param argv An array of the arguments given through the command line.
 * @return 0 if the program ran as intended, 1 otherwise.
 */
int main(int argc, char *argv[])
{
    if (argc != NUM_ARGS) // Check valid number of arguments.
    {
        fprintf(stderr, USAGE_MSG);
        return FALSE;
    }
    FILE *fp;
    fp = fopen(argv[1], READ);
    if (fp == NULL) // Check that the open operation succeeded.
    {
        fprintf(stderr, INPUT_MSG);
        return FALSE;
    }
    char buffer[MAX_LINE]; // Create buffer.
    if (fgets(buffer, MAX_LINE, fp) == NULL) // The file is empty.
    {
        fprintf(stderr, INPUT_MSG);
        fclose(fp); // Close the given file.
        return FALSE;
    }
    sscanf(buffer, "%[^\r\n]", buffer);
    int vertices = checkFirstLine(buffer); // Get number of nodes.
    if (vertices == DEFAULT_VAL) // The first line was invalid.
    {
        fprintf(stderr, INPUT_MSG);
        fclose(fp); // Close the given file.
        return FALSE;
    }
    Tree tree;
    tree.maxDKEy = DEFAULT_VAL;
    tree.vertices = vertices;
    Node *treeArr = (Node *) malloc(vertices * sizeof(Node));
    // Allocate space for all the nodes of the tree based on the amount read in line 1.
    if (treeArr == NULL) // Check that the malloc operation succeeded.
    {
        fprintf(stderr, MEMORY_MSG);
        fclose(fp); // Close the given file.
        return FALSE;
    }
    int inVal = initialize(&treeArr, vertices);
    if (inVal == MEMORY_PROB) // Check that the malloc operation succeeded.
    {
        fprintf(stderr, MEMORY_MSG);
        freeAll(&treeArr, tree.vertices); // Free all memory.
        fclose(fp);
        return FALSE;
    }
    if (checkValidNodes(&argv, vertices) == FALSE) // Check that the given vertices are valid.
    {
        fprintf(stderr, INPUT_MSG);
        freeAll(&treeArr, tree.vertices); // Free all memory.
        fclose(fp);
        return FALSE;
    }
    int iter = 0;
    while (fgets(buffer, MAX_LINE, fp) != NULL)
    {
        sscanf(buffer, "%[^\r\n]", buffer);
        int num = checkSons(buffer, tree.vertices, iter, &treeArr);
        if (num == FALSE)
        {
            fprintf(stderr, INPUT_MSG);
            freeAll(&treeArr, tree.vertices); // Free all memory.
            fclose(fp);
            return FALSE;
        }
        else if (num == MEMORY_PROB)
        {
            fprintf(stderr, MEMORY_MSG);
            freeAll(&treeArr, tree.vertices); // Free all memory.
            fclose(fp);
            return FALSE;
        }
        iter++;
    }
    if (iter < vertices || iter > vertices + 1) // Not enough lines found or too much.
    {
        fprintf(stderr, INPUT_MSG);
        freeAll(&treeArr, tree.vertices); // Free all memory.
        fclose(fp);
        return FALSE;
    }
    tree.root = findRoot(&treeArr, vertices);
    printf(REGEX, ROOT_MSG, tree.root); // Prints the root key.
    printf(REGEX, VERTI_MSG, tree.vertices); // Prints the number of nodes.
    printf(REGEX, EDGES_MSG, tree.vertices - 1); // Prints the number of edges.
    bfs(&treeArr, tree.root, tree.vertices);
    int minD = getHeight(&treeArr, tree.vertices, &tree, 1);
    printf(REGEX, MIN_LEN_MSG, minD); // Prints the minimal height.
    int maxD = getHeight(&treeArr, tree.vertices, &tree, 0);
    printf(REGEX, MAX_LEN_MSG, maxD); // Prints the maximal height.
    bfs(&treeArr, tree.maxDKEy, tree.vertices);
    int diam = getHeight(&treeArr, tree.vertices, &tree, 0);
    printf(REGEX, DIAM_MSG, diam); // Prints the maximal diameter.
    printf("%s%d%s%d%s", PATH_MSG, \
    (int) strtol(argv[2], NULL, 10), " and ", (int) strtol(argv[3], NULL, 10), ": ");
    int nodeOne = (int) strtol(argv[2], NULL, 10);
    int nodeTwo = (int) strtol(argv[3], NULL, 10);
    bfs(&treeArr, nodeTwo, tree.vertices);
    printPath(&treeArr, nodeOne, nodeTwo);
    freeAll(&treeArr, tree.vertices); // Free all memory.
    fclose(fp);
    return TRUE;
}