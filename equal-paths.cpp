#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

bool calcleafDepth(Node *node, int currDepth, int& leafDepth) {
    if (node == nullptr) {
        return true;
    }

    if (node->left == nullptr && node->right == nullptr) {
        if (leafDepth == -1) {
            leafDepth = currDepth;
            return true;
        }
        return currDepth == leafDepth;
    }

    return calcleafDepth(node->left, currDepth + 1, leafDepth) && 
    calcleafDepth(node->right, currDepth + 1, leafDepth);
}

bool equalPaths(Node * root)
{
    // Add your code below
    int leafDepth = -1;
    return calcleafDepth(root, 0, leafDepth);

}

