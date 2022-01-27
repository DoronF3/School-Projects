#include "Structs.h"
#include <string.h>
#include <stdlib.h>

#define FAIL 0
#define SUCCESS 1

/**
 * CompFunc for strings (assumes strings end with "\0")
 * @param a - char* pointer
 * @param b - char* pointer
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a. (lexicographic
 * order)
 */
int stringCompare(const void *a, const void *b)
{
    return strcmp((char *) a, (char *) b);
}

/**
 * ForEach function that concatenates the given word to pConcatenated. pConcatenated is already allocated with
 * enough space.
 * @param word - char* to add to pConcatenated
 * @param pConcatenated - char*
 * @return 0 on failure, 1 on success
 */
int concatenate(const void *word, void *pConcatenated)
{
    if (word == NULL || pConcatenated == NULL)
    {
        return FAIL;
    }
    char *pWord = strcat((char *) pConcatenated, (char *) word);
    if (pWord == NULL)
    {
        return FAIL;
    }
    pWord = strcat((char *) pConcatenated, "\n");
    if (pWord == NULL)
    {
        return FAIL;
    }
    return SUCCESS;
}

/**
 * FreeFunc for strings
 */
void freeString(void *s)
{
    free(s);
}

/**
 * CompFunc for Vectors, compares element by element, the vector that has the first larger
 * element is considered larger. If vectors are of different lengths and identify for the length
 * of the shorter vector, the shorter vector is considered smaller.
 * @param a - first vector
 * @param b - second vector
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a.
 */
int vectorCompare1By1(const void *a, const void *b)
{
    Vector *v1 = (Vector *) a;
    Vector *v2 = (Vector *) b;
    int i = 0, j = 0;
    while (i < v1->len && j < v2->len)
    {
        if (v1->vector[i] > v2->vector[i])
        {
            return 1;
        }
        if (v1->vector[i] < v2->vector[i])
        {
            return -1;
        }
        i++;
        j++;
    }
    if (i < v1->len && j == v2->len) // Vector a is longer.
    {
        return 1;
    }
    if (j < v2->len) // Vector b is longer.
    {
        return -1;
    }
    return 0; // Equal.
}

/**
 * FreeFunc for vectors
 */
void freeVector(void *pVector)
{
    if (pVector == NULL)
    {
        return;
    }
    free(((Vector *) pVector)->vector);
    ((Vector *) pVector)->vector = NULL;
    free(pVector);
    pVector = NULL;
}

/**
 * A method that calculates the power of norm of a vector and returns it.
 * @param pVector A pointer to the vector we need to calculate.
 * @return The power of the norm of the vector.
 */
double calcNorm(const void *pVector)
{
    Vector *v1 = (Vector *) pVector;
    double norm = 0;
    if (v1->len < 0)
    {
        return norm;
    }
    for (int i = 0; i < v1->len; i++)
    {
        norm += (v1->vector[i] * v1->vector[i]);
    }
    return norm;
}

/**
 * copy pVector to pMaxVector if : 1. The norm of pVector is greater then the norm of pMaxVector.
 * 								   2. pMaxVector == NULL.
 * @param pVector pointer to Vector
 * @param pMaxVector pointer to Vector
 * @return 1 on success, 0 on failure (if pVector == NULL: failure).
 */
int copyIfNormIsLarger(const void *pVector, void *pMaxVector)
{
    if (pMaxVector == NULL)
    {
        return FAIL;
    }
    if (pVector == NULL || ((Vector *) pVector)->vector == NULL)
    {
        return FAIL;
    }
    double norm1 = 0, norm2 = 0;
    norm1 = calcNorm(pVector);
    norm2 = calcNorm(pMaxVector);
    if (((Vector *) pMaxVector)->vector == NULL || norm1 > norm2)
    {
        Vector *v1 = (Vector *) pMaxVector;
        Vector *v2 = (Vector *) pVector;
        if (v1->vector == NULL)
        {
            v1->vector = (double *) malloc((v2->len * sizeof(double)));
        }
        else
        {
            v1->vector = (double *) realloc(v1->vector, v2->len * sizeof(double));
        }
        if (v1->vector == NULL)
        {
            return FAIL;
        }
        memcpy(v1->vector, v2->vector, v2->len * sizeof(double));
        v1->len = v2->len;
    }
    return SUCCESS;
}

/**
 * @param tree a pointer to a tree of Vectors
 * @return pointer to a *copy* of the vector that has the largest norm (L2 Norm).
 */
Vector *findMaxNormVectorInTree(RBTree *tree)
{
    if (tree == NULL || tree->root == NULL || tree->root->data == NULL)
    {
        return NULL;
    }
    Vector *maxV = (Vector *) malloc(sizeof(Vector));
    if (maxV == NULL)
    {
        return NULL;
    }
    maxV->len = 0;
    maxV->vector = NULL;
    int num = forEachRBTree(tree, copyIfNormIsLarger, maxV);
    if (num == 0)
    {
        freeVector(maxV);
        maxV = NULL;
        return NULL;
    }
    return maxV;
}
