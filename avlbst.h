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

    AVLNode<Key, Value>* insertRec(Node<Key, Value> *node, Node<Key, Value> *parent, const std::pair<const Key, Value> &keyValuePair) override;
    AVLNode<Key, Value>* rebalance(AVLNode<Key, Value> *node);
    AVLNode<Key, Value>* rightRotate(AVLNode<Key, Value> *p);
    AVLNode<Key, Value>* leftRotate(AVLNode<Key, Value> *p);
    AVLNode<Key, Value>* leftRightRotate(AVLNode<Key, Value> *p);
    AVLNode<Key, Value>* rightLeftRotate(AVLNode<Key, Value> *p);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */

template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    this->root_ = insertRec(this->root_, NULL, new_item);

}

template<typename Key, typename Value>
AVLNode<Key, Value>* 
AVLTree<Key, Value>::insertRec(Node<Key, Value> *node, Node<Key,Value> *parent, const std::pair<const Key, Value> &keyValuePair) {
    
    // logic for inserting node at end of tree
    if (node == NULL) {
        return new AVLNode<Key, Value>(keyValuePair.first, keyValuePair.second, parent);
    }

    AVLNode<Key, Value>* avlNode = static_cast<AVLNode<Key, Value>*>(node);

    if (keyValuePair.first < avlNode->getKey()) {
        avlNode->setLeft(insertRec(avlNode->getLeft(), avlNode, keyValuePair));
    } else if (keyValuePair.first > avlNode->getKey()) {
        avlNode->setRight(insertRec(avlNode->getRight(), avlNode, keyValuePair));
    } else {
        avlNode->setValue(keyValuePair.second);
        return avlNode;
    }

    // back-tracking logic
    return rebalance(avlNode);
}

template<typename Key, typename Value>
 AVLNode<Key, Value>* 
 AVLTree<Key,Value>::rebalance(AVLNode<Key, Value> *node) {
    if (node == NULL) {
        return NULL;
    }

    if (node->getParent()) {
        if (node == node->getParent()->getLeft()) {
            node->getParent()->updateBalance(-1);
        } else if (node == node->getParent()->getRight()) {
            node->getParent()->updateBalance(1);
        }
    }

    if (node->getBalance() > 1) { // right-heavy
        if (node->getRight()->getBalance() < 0) {
            return rightLeftRotate(node);
        }
        return leftRotate(node);

    } else if (node->getBalance() < -1) { // left-heavy
        if (node->getLeft()->getBalance() > 0) {
            return leftRightRotate(node);
        }
        return rightRotate(node);
    }

    return node;
 }
 
 template<typename Key, typename Value>
 AVLNode<Key, Value>* 
 AVLTree<Key, Value>::rightRotate(AVLNode<Key, Value> *p) {
    AVLNode<Key, Value>* n = p->getLeft();
    
    p->setLeft(n->getRight());
    if (n->getRight() != NULL) {
        n->getRight()->setParent(p);
    }

    n->setParent(p->getParent());

    if (p->getParent() == NULL) { // if p is the root
        this->root_ = n;
    } else if (p == p->getParent()->getRight()) { // if p is right child
        p->getParent()->setRight(n);
    } else if (p == p->getParent()->getLeft()) { // if p is left child
        p->getParent()->setLeft(n);
    }

    n->setRight(p);
    p->setParent(n);

    n->updateBalance(1);

    if (p->getLeft()) {
        p->updateBalance(1);
    } else {
        p->updateBalance(2);
    }

    return n;
 }

 template<typename Key, typename Value>
 AVLNode<Key, Value>* 
 AVLTree<Key, Value>::leftRotate(AVLNode<Key, Value> *p) {
    AVLNode<Key, Value>* n = p->getRight();
    
    p->setRight(n->getLeft());
    if (n->getLeft() != NULL) {
        n->getLeft()->setParent(p);
    }

    n->setParent(p->getParent());

    if (p->getParent() == NULL) { // if p is root
        this->root_ = n;
    } else if (p == p->getParent()->getRight()) { // p is right child
        p->getParent()->setRight(n);
    } else if (p == p->getParent()->getLeft()) { // p is left child
        p->getParent()->setLeft(n);
    }

    n->setLeft(p);
    p->setParent(n);

    n->updateBalance(-1);
    if (p->getRight()) {
        p->updateBalance(-1);
    } else {
        p->updateBalance(-2);
    }

    return n;
 }

 template<typename Key, typename Value>
 AVLNode<Key, Value>* 
 AVLTree<Key, Value>::leftRightRotate(AVLNode<Key, Value> *p) {
    p->setLeft(leftRotate(p->getLeft()));
    return rightRotate(p);
 }

 template<typename Key, typename Value>
 AVLNode<Key, Value>* 
 AVLTree<Key, Value>::rightLeftRotate(AVLNode<Key, Value> *p) {
    p->setRight(rightRotate(p->getRight()));
    return leftRotate(p);
 }

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
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
