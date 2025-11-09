#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
bool equalPathsHelper(Node * root, int depth, int& leafDepth) 
{
  // Base Cases
  // Root is null
  if (root == nullptr)
  {
    return true;
  }
  // At a leaf (i.e. left and right are null)
  else if (root->left == nullptr && root->right == nullptr)
  {
    // if leafDepth hasn't been set yet, set it to depth, since this is an leaf
    if (leafDepth == -1)
    {
      leafDepth = depth;
      return true;
    }

    // Otherwise return true if depth is the leafDepth
    else
    {
      return depth == leafDepth;
    }
  }
  // Recurse to left and right of node with updated depth
  else
  {
    return equalPathsHelper(root->left, depth + 1, leafDepth) && equalPathsHelper(root->right, depth + 1, leafDepth);
  }

}

bool equalPaths(Node * root)
{
  // Add your code below

  // initialize leafDepth
  int leafDepth = -1;

  return equalPathsHelper(root, 0, leafDepth);

}

