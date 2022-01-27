#include "RBTree.h"
#include <stdlib.h>
#include <stdio.h>

#define RIGHT 'r'
#define LEFT 'l'
#define RL 'k'
#define LR 'f'
#define RR 'q'
#define LL 't'
#define FAIL 0
#define SUCCESS 1

/**
 * Constructs a new RBTree with the given CompareFunc.
 * @param compFunc A pointer to a compare function.
 * @param freeFunc A pointer to a free function.
 * @return A pointer to the new tree or NULL if failed.
 */
RBTree *newRBTree(CompareFunc compFunc, FreeFunc freeFunc)
{
    if (compFunc == NULL || freeFunc == NULL)
    {
        return NULL;
    }
    RBTree *tree = (RBTree *) malloc(sizeof(RBTree));
    if (tree == NULL)
    {
        return NULL;
    }
    tree->root = NULL;
    tree->size = 0;
    tree->compFunc = compFunc;
    tree->freeFunc = freeFunc;
    return tree;
}

/**
 * A method that performs left rotation on a tree.
 * @param n The given node that acts as the axis to the rotation.
 */
void leftRotation(Node *node, RBTree *tree)
{
    Node *temp = node->right;
    node->right = temp->left;

    if (temp->left != NULL)
    {
        temp->left->parent = node;
    }
    temp->parent = node->parent;

    if (node->parent == NULL)
    {
        tree->root = temp;
    }
    else if (node->parent->left == node)
    {
        node->parent->left = temp;
    }
    else
    {
        node->parent->right = temp;
    }
    temp->left = node;
    node->parent = temp;
}

/**
 * A method that performs right rotation on a tree.
 * @param n The given node that acts as the axis to the rotation.
 */
void rightRotation(Node *node, RBTree *tree)
{
    Node *temp = node->left;
    node->left = temp->right;

    if (temp->right != NULL)
    {
        temp->right->parent = node;
    }
    temp->parent = node->parent;

    if (node->parent == NULL)
    {
        tree->root = temp;
    }
    else if (node->parent->right == node)
    {
        node->parent->right = temp;
    }
    else
    {
        node->parent->left = temp;
    }
    temp->right = node;
    node->parent = temp;
}

/**
 * A method that balances the given tree given all the relevant node.
 * @param flag A given character to represent the position of the node.
 * @param parent The parent of the node.
 * @param node A node to check.
 * @param grand The grandparent of the node.
 */
void balance(char flag, Node *parent, Node *grand, RBTree *tree)
{
    parent->color = BLACK;

    if (flag != RR && flag != LL)
    {
        if (flag == LR)
        {
            parent->right->color = BLACK;
            leftRotation(parent, tree);
            rightRotation(grand, tree);
            parent->color = RED;
            grand->color = RED;
        }
        else
        {
            parent->left->color = BLACK;
            rightRotation(parent, tree);
            leftRotation(grand, tree);
            parent->color = RED;
            grand->color = RED;
        }
    }
    else if (flag == RR)
    {
        leftRotation(grand, tree);
        parent->color = BLACK;
        grand->color = RED;
    }
    else
    {
        rightRotation(grand, tree);
        parent->color = BLACK;
        grand->color = RED;
    }
}

/**
 * A method that checks the state of the tree between 3 possible situations,
 * which are documented on the exercise pdf.
 * @param node A node to check.
 * @param parent The parent of the node.
 * @param flag A given character to represent the position of the node.
 */
void fix(CompareFunc func, Node *node, Node *parent, char flag, RBTree *tree)
{
    if (parent == NULL) // Situation 1.
    {
        node->color = BLACK;
        tree->root = node;
        return;
    }
    if (parent->color == BLACK) // Situation 2.
    {
        return;
    }
    Node *grand = parent->parent;
    if (grand == NULL) // Check we have grand.
    {
        return;
    }
    Node *uncle = NULL;
    if (func(parent->data, grand->data) < 0) // Parent is left son of grand.
    {
        uncle = grand->right; // Set uncle to right son of grand.
        if (flag == RIGHT)
        {
            flag = LR;
        }
        else
        {
            flag = LL;
        }
    }
    else
    {
        uncle = grand->left;
        if (flag == RIGHT)
        {
            flag = RR;
        }
        else
        {
            flag = RL;
        }
    }
    Color color;
    if (uncle != NULL)
    {
        color = uncle->color;
    }
    else
    {
        color = BLACK;
    }
    if (parent->color == RED && color == RED) // Situation 3.
    {
        parent->color = BLACK;
        uncle->color = BLACK;
        grand->color = color;
        if (grand->parent != NULL && grand->parent->right == grand)
        {
            flag = RIGHT;
        }
        else if (grand->parent != NULL)
        {
            flag = LEFT;
        }
        fix(func, grand, grand->parent, flag, tree); // Try to fix again.
    }
    else if (parent->color == RED && color == BLACK) // Situation 4
    {
        balance(flag, parent, grand, tree); // Balance the tree.
    }
}

/**
 * Add an item to the tree.
 * @param tree A pointer to a tree.
 * @param data A pointer to data.
 * @return 0 if failed, 1 otherwise.
 */
int addToRBTree(RBTree *tree, void *data)
{
    char flag = '0';
    if (tree == NULL || data == NULL)
    {
        return FAIL;
    }
    Node *cur = tree->root;
    Node *parent = NULL;
    while (cur != NULL)
    {
        if (tree->compFunc(data, cur->data) > 0) // Go right.
        {
            parent = cur;
            cur = cur->right;
            flag = RIGHT;
        }
        else if (tree->compFunc(data, cur->data) < 0) // Go left.
        {
            parent = cur;
            cur = cur->left;
            flag = LEFT;
        }
        else // Equal, return fail.
        {
            return FAIL;
        }
    }
    Node *node = (Node *) malloc(sizeof(Node));
    if (node == NULL) // Check malloc.
    {
        return FAIL;
    }
    node->data = data; // Set data.
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->color = RED;
    if (parent == NULL) // The node is root.
    {
        tree->root = node; // Set root of tree.
    }
    else if (flag == RIGHT) // Node is right son.
    {
        parent->right = node;
        node->parent = parent;
    }
    else // Node is left son.
    {
        parent->left = node;
        node->parent = parent;
    }
    tree->size++; // Increment size of tree.
    fix(tree->compFunc, node, parent, flag, tree); // Check if a fix is needed.
    return SUCCESS;
}

/**
 * Check whether the tree contains this item.
 * @param tree: The tree to add an item to.
 * @param data: Item to check.
 * @return: 0 if the item is not in the tree, 1 if it is.
 */
int containsRBTree(RBTree *tree, void *data)
{
    if (tree == NULL || data == NULL)
    {
        return FAIL;
    }
    Node *node = tree->root;
    while (node != NULL)
    {
        if (tree->compFunc(data, node->data) == 0)
        {
            return SUCCESS;
        }
        else if (tree->compFunc(data, node->data) > 0)
        {
            node = node->right;
        }
        else
        {
            node = node->left;
        }
    }
    return FAIL;
}

/**
 * A helper method that goes through the nodes in ascending order.
 * @param node A node.
 * @param func The function to activate on all items.
 * @param args More optional arguments to the function (may be null if the given function support it).
 * @return 0 on failure, 1 on success.
 */
int helper(Node *node, forEachFunc func, void *args)
{
    if (node->left != NULL)
    {
        int s = helper(node->left, func, args);
        if (s == 0)
        {
            return FAIL;
        }
    }
    int num = func(node->data, args);
    if (num == 0)
    {
        return FAIL;
    }
    if (node->right != NULL)
    {
        int s = helper(node->right, func, args);
        if (s == 0)
        {
            return FAIL;
        }
    }
    return SUCCESS;
}

/**
 * Activate a function on each item of the tree. the order is an ascending order. if one of the activations of the
 * function returns 0, the process stops.
 * @param tree: The tree with all the items.
 * @param func: The function to activate on all items.
 * @param args: More optional arguments to the function (may be null if the given function support it).
 * @return: 0 on failure, 1 on success.
 */
int forEachRBTree(RBTree *tree, forEachFunc func, void *args)
{
    if (tree == NULL || func == NULL)
    {
        return FAIL;
    }
    if (tree->root == NULL)
    {
        return SUCCESS;
    }
    return helper(tree->root, func, args);
}

/**
 * A method that frees all the memory allocations of the nodes.
 * @param node The node we receive.
 */
void freeNode(struct Node *node, FreeFunc func)
{
    if (node->right == NULL && node->left == NULL) // Node is leaf.
    {
        func(node->data);
        node->data = NULL;
        free(node);
        node = NULL;
        return;
    }
    else if (node->right == NULL) // Node has only left son.
    {
        freeNode(node->left, func);
    }
    else if (node->left == NULL) // Node has only right son.
    {
        freeNode(node->right, func);
    }
    else // Node has two sons.
    {
        freeNode(node->right, func);
        freeNode(node->left, func);
    }
    func(node->data);
    node->data = NULL;
    free(node); // Free the current node.
    node = NULL; // Set pointer to NULL.
}

/**
 * Free all memory of the data structure.
 * @param tree: The tree to free.
 */
void freeRBTree(RBTree *tree)
{
    if (tree == NULL)
    {
        return;
    }
    freeNode(tree->root, tree->freeFunc);
    free(tree);
    tree = NULL;
}

