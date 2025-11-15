#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void rebalanceInsert(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* newNode);
    void rebalanceRemove(AVLNode<Key, Value>* parent, bool deletedFromLeft);
    void rotateLeft(AVLNode<Key, Value>* x);
    void rotateRight(AVLNode<Key, Value>* x);


};

/*
 * rebalance the BST after insert to make the AVL tree
 */
template<class Key, class Value>
void AVLTree<Key, Value>::rebalanceInsert (AVLNode<Key, Value>* parent, AVLNode<Key, Value>* newNode)
{
  AVLNode<Key, Value>* p = parent;
  // AVLNode<Key, Value>* n = newNode;

  while (p != NULL)
  {
    int8_t balance = p->getBalance();

    if (balance == 0)
    {
      break;
    }

    else if (balance == -1 || balance == 1)
    {
      AVLNode<Key, Value>* grandparent = p->getParent();

      if (grandparent != NULL)
      {
        if (p == grandparent->getLeft())
        {
          grandparent->updateBalance(+1);
        }
        else
        {
          grandparent->updateBalance(-1);
        }
      }

      // n = p;
      p = grandparent;

      continue;
    }

    // left heavy
    else if (balance == 2)
    {
      AVLNode<Key, Value>* parentLeft = p->getLeft();

      if (parentLeft->getBalance() >= 0)
      {
        rotateRight(p);
        p->setBalance(0);
        parentLeft->setBalance(0);
      }

      else
      {
        AVLNode<Key, Value>* parentLeftRight = parentLeft->getRight();
        rotateLeft(parentLeft);
        rotateRight(p);

        if(parentLeftRight->getBalance() == +1)
        {
          p->setBalance(-1);
          parentLeft->setBalance(0);
        }
        else if (parentLeftRight->getBalance() == 0)
        {
          p->setBalance(0);
          parentLeft->setBalance(0);
        }
        else // parentLeftRight->getBalance() == -1
        {
          p->setBalance(0);
          parentLeft->setBalance(+1);
        }

        parentLeftRight->setBalance(0);
      }
      break;
    }

    // right heavy
    else
    {
      AVLNode<Key, Value>* parentRight = p->getRight();

      if (parentRight->getBalance() <= 0)
      {
        rotateLeft(p);
        p->setBalance(0);
        parentRight->setBalance(0);
      }

      else
      {
        AVLNode<Key, Value>* parentRightLeft = parentRight->getLeft();
        rotateRight(parentRight);
        rotateLeft(p);

        if(parentRightLeft->getBalance() == -1)
        {
          p->setBalance(+1);
          parentRight->setBalance(0);
        }
        else if (parentRightLeft->getBalance() == 0)
        {
          p->setBalance(0);
          parentRight->setBalance(0);
        }
        else // parentRightLeft->getBalance() == +1
        {
          p->setBalance(0);
          parentRight->setBalance(-1);
        }

        parentRightLeft->setBalance(0);
      }
      break;
    }
  }
}

/*
 * rebalance the BST after remove to make the AVL tree
 */
template<class Key, class Value>
void AVLTree<Key, Value>::rebalanceRemove (AVLNode<Key, Value>* parent, bool deletedFromLeft)
{
  AVLNode<Key, Value>* p = parent;
  bool dfl = deletedFromLeft;

  while (p != NULL)
  {
    if (dfl)
    {
      p->updateBalance(-1);
    }
    else
    {
      p->updateBalance(+1);
    }
    
    int8_t balance = p->getBalance();

    if (balance == -1 || balance == +1)
    {
      break;
    }

    else if (balance == 0)
    {
      AVLNode<Key, Value>* grandparent = p->getParent();

      if (grandparent == NULL)
      {
        break;
      }

      else
      {
        if (p == grandparent->getLeft())
        {
          dfl = true;
        }
        else
        {
          dfl = false;
        }
        p = grandparent;
        continue;
      }
      
    }

    // left heavy
    else if (balance == +2)
    {
      AVLNode<Key, Value>* parentLeft = p->getLeft();
      int8_t leftBalance = parentLeft->getBalance();

      if (leftBalance >= 0)
      {
        rotateRight(p);

        if (leftBalance == 0)
        {
          p->setBalance(+1);
          parentLeft->setBalance(-1);
          break;
        }
        else
        {
          p->setBalance(0);
          parentLeft->setBalance(0);

          AVLNode<Key, Value>* grandparent = parentLeft->getParent();

          if (grandparent == NULL)
          {
            break;
          }

          else
          {
            if (parentLeft == grandparent->getLeft())
            {
              dfl = true;
            }
            else
            {
              dfl = false;
            }
            p = grandparent;
            continue;
          }
        }
      }

    
      else
      {
        AVLNode<Key, Value>* parentLeftRight = parentLeft->getRight();
        int8_t leftRightBalance = parentLeftRight->getBalance();
        
        rotateLeft(parentLeft);
        rotateRight(p);

        if(leftRightBalance == +1)
        {
          p->setBalance(-1);
          parentLeft->setBalance(0);
        }
        else if (leftRightBalance == 0)
        {
          p->setBalance(0);
          parentLeft->setBalance(0);
        }
        else // leftRightBalance == -1
        {
          p->setBalance(0);
          parentLeft->setBalance(+1);
        }

        parentLeftRight->setBalance(0);

        AVLNode<Key, Value>* grandparent = parentLeftRight->getParent();

        if (grandparent == NULL)
        {
          break;
        }

        else
        {
          if (parentLeftRight == grandparent->getLeft())
          {
            dfl = true;
          }
          else
          {
            dfl = false;
          }
          p = grandparent;
          continue;
        }
      }
    }

    // right heavy
    else
    {
      AVLNode<Key, Value>* parentRight = p->getRight();
      int8_t rightBalance = parentRight->getBalance();

      if (rightBalance <= 0)
      {
        rotateLeft(p);

        if (rightBalance == 0)
        {
          p->setBalance(-1);
          parentRight->setBalance(+1);
          break;
        }
        else
        {
          p->setBalance(0);
          parentRight->setBalance(0);

          AVLNode<Key, Value>* grandparent = parentRight->getParent();

          if (grandparent == NULL)
          {
            break;
          }

          else
          {
            if (parentRight == grandparent->getLeft())
            {
              dfl = true;
            }
            else
            {
              dfl = false;
            }
            p = grandparent;
            continue;
          }
        }
      }

    
      else
      {
        AVLNode<Key, Value>* parentRightLeft = parentRight->getLeft();
        int8_t rightLeftBalance = parentRightLeft->getBalance();
        
        rotateRight(parentRight);
        rotateLeft(p);

        if(rightLeftBalance == -1)
        {
          p->setBalance(+1);
          parentRight->setBalance(0);
        }
        else if (rightLeftBalance == 0)
        {
          p->setBalance(0);
          parentRight->setBalance(0);
        }
        else // rightLeftBalance == -1
        {
          p->setBalance(0);
          parentRight->setBalance(-1);
        }

        parentRightLeft->setBalance(0);

        AVLNode<Key, Value>* grandparent = parentRightLeft->getParent();

        if (grandparent == NULL)
        {
          break;
        }

        else
        {
          if (parentRightLeft == grandparent->getLeft())
          {
            dfl = true;
          }
          else
          {
            dfl = false;
          }
          p = grandparent;
          continue;
        }
      }
    }
  }
}

/*
 * rotate node x to the left
 */
template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft (AVLNode<Key, Value>* x)
{
  AVLNode<Key, Value>* y = x->getRight();
  x->setRight(y->getLeft());

  if (y->getLeft() != NULL)
  {
    y->getLeft()->setParent(x);
  }

  y->setParent(x->getParent());

  if (x->getParent() == NULL)
  {
    this->root_ = y;
  }
  else if (x == x->getParent()->getLeft())
  {
    x->getParent()->setLeft(y);
  }
  else
  {
    x->getParent()->setRight(y);
  }

  y->setLeft(x);
  x->setParent(y);


}

/*
 * rotate node x to the right
 */
template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight (AVLNode<Key, Value>* x)
{
  AVLNode<Key, Value>* y = x->getLeft();
  x->setLeft(y->getRight());

  if (y->getRight() != NULL)
  {
    y->getRight()->setParent(x);
  }

  y->setParent(x->getParent());

  if (x->getParent() == NULL)
  {
    this->root_ = y;
  }
  else if (x == x->getParent()->getRight())
  {
    x->getParent()->setRight(y);
  }
  else
  {
    x->getParent()->setLeft(y);
  }

  y->setRight(x);
  x->setParent(y);
}

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    // Base Case: Tree is empty
    if (this->root_ == NULL)
    {
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
        return;
    }

    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value>* parent = NULL;

    // check place of new_item as if it was a BST
    while (curr != NULL)
    {
      parent = curr;
      
      if (new_item.first < curr->getKey())
      {
        curr = curr->getLeft();
      }
      else if (new_item.first > curr->getKey())
      {
        curr = curr->getRight();
      }
      else
      {
        curr->setValue(new_item.second);
        return;
      }
    }

    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);

    // attaching new node
    if (new_item.first < parent->getKey())
    {
        parent->setLeft(newNode);
    }
    else
    {
        parent->setRight(newNode);
    }

    // update balancing
    if (newNode == parent->getLeft())
    {
        parent->updateBalance(1);
    }
    else
    {
        parent->updateBalance(-1);
    }

    rebalanceInsert(parent, newNode);
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    AVLNode<Key, Value>* selected = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));

    if (selected == NULL)
    {
      return;
    }
    if (selected->getLeft() != NULL && selected->getRight() != NULL)
    {
      AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(this->predecessor(selected));
      nodeSwap(selected, temp);
    }
    
    AVLNode<Key, Value>* child = NULL;
    AVLNode<Key, Value>* parent = selected->getParent();
    bool deletedFromLeft = false;

    if (selected->getLeft() != NULL)
    {
      child = selected->getLeft();
    }
    else
    {
      child = selected->getRight();
    }


    if (child != NULL)
    {
      child->setParent(parent);
    }

    if (parent == NULL)
    {
      this->root_ = child;
    }
    else if (parent->getLeft() == selected)
    {
      parent->setLeft(child);
      deletedFromLeft = true;
    }
    else if (parent->getRight() == selected)
    {
      parent->setRight(child);
      deletedFromLeft = false;
    }

    delete selected;

    // rebalance if needed
    if (parent != NULL)
    {
      rebalanceRemove(parent, deletedFromLeft);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
