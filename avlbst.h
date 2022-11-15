#ifndef RBBST_H
#define RBBST_H

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
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
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
    virtual void insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* n); // TODO 
    virtual void removeFix(AVLNode<Key, Value>* n, int8_t diff);  // TODO
    virtual void rotateRight(AVLNode<Key, Value>* cur);
    virtual void rotateLeft(AVLNode<Key, Value>* cur);

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{
    // TODO
  //call bst_insert 
  //BinarySearchTree<Key, Value>::insert(new_item);
  if(this->root_ == NULL){
    AVLNode<Key, Value>* new_node = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
    this->root_ = new_node;
    return;
  }
  else{
    AVLNode<Key, Value>* root_temp = static_cast<AVLNode<Key, Value>*>(this->root_);
    while(1){
      if(new_item.first > root_temp->getKey()){
        //there is no right child
        if(root_temp->getRight() == NULL){
          //then we can make a new node
          AVLNode<Key, Value>* new_node = new AVLNode<Key, Value>(new_item.first, new_item.second, root_temp);
          root_temp->setRight(new_node);
          break; //done with inserting

        }
        root_temp = root_temp->getRight();
      }
      else if(new_item.first < root_temp->getKey()){
        if(root_temp->getLeft() == NULL){
          AVLNode<Key, Value>* new_node = new AVLNode<Key, Value>(new_item.first, new_item.second, root_temp);
          root_temp->setLeft(new_node);
          break;
        }
        root_temp = root_temp->getLeft();
      }
      else{ 
        //overwrite the current value with the updated value
        root_temp->setValue(new_item.second);
        break;
      }

    }
  }
  AVLNode<Key, Value>* new_node = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::internalFind(new_item.first));
  AVLNode<Key, Value>* parent = new_node->getParent();
  new_node->setBalance(0);
  if(parent == NULL){
    return;
  }
  if(parent->getBalance() == -1 || parent->getBalance() == 1){
    parent->setBalance(0);
  }
  else if(parent->getBalance() == 0){
    if(new_node->getKey() < parent->getKey()){
      parent->updateBalance(-1);
      if(parent->getParent() == NULL){
        return;
      }
      insertFix(parent, new_node);
    }
    else if(new_node->getKey() > parent->getKey()){
      parent->updateBalance(1);
      if(parent->getParent() == NULL){
        return;
      }
      //parent->getParent()->updateBalance(1);
      insertFix(parent, new_node);
    }
  }

}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* n)
{
    // TODO
  if((parent->getBalance() == 1 || parent->getBalance() == 0 || parent->getBalance() == -1) && (n->getBalance() == 1 || n->getBalance() == 0 || n->getBalance() == -1)){
    if((parent == NULL) || (parent->getParent() == NULL)){
      return;
    }
    AVLNode<Key, Value>* grandP = parent->getParent();
    if(grandP->getLeft() == parent){
      grandP->updateBalance(-1);
      if(grandP->getBalance() == 0){
        return;
      }
      else if(grandP->getBalance() == -1){
        insertFix(grandP, parent);
      }
      else if(grandP->getBalance() == -2){
        if(grandP->getLeft() == parent && parent->getLeft() == n){ //zig zig case
          rotateRight(grandP);
          grandP->setBalance(0);
          parent->setBalance(0);
        }
        else if(grandP->getLeft() == parent && parent->getRight() == n){ //zig zag case
          rotateLeft(parent);
          rotateRight(grandP);
          if(n->getBalance() == -1){
            parent->setBalance(0);
            grandP->setBalance(1);
            n->setBalance(0);
          }
          else if(n->getBalance() == 0){
            parent->setBalance(0);
            grandP->setBalance(0);
            n->setBalance(0);
          }
          else if(n->getBalance() == 1){
            parent->setBalance(-1);
            grandP->setBalance(0);
            n->setBalance(0);
          }
        }
      }
    }
    else if(grandP->getRight() == parent){
      grandP->updateBalance(1);
      if(grandP->getBalance() == 0){
        return;
      }
      else if(grandP->getBalance() == 1){
        insertFix(grandP, parent);
      }
      else if(grandP->getBalance() == 2){
        if(grandP->getRight() == parent && parent->getRight() == n){ //zig zig case
          rotateLeft(grandP);
          grandP->setBalance(0);
          parent->setBalance(0);
        }
        if(grandP->getRight() == parent && parent->getLeft() == n){ //zig zag case
          rotateRight(parent);
          rotateLeft(grandP);
          if(n->getBalance() == 1){
            parent->setBalance(0);
            grandP->setBalance(-1);
            n->setBalance(0);
          }
          else if(n->getBalance() == 0){
            parent->setBalance(0);
            grandP->setBalance(0);
            n->setBalance(0);
          }
          else if(n->getBalance() == -1){
            parent->setBalance(1);
            grandP->setBalance(0);
            n->setBalance(0);
          }
        }
      }
    }
  }
  
}
template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* cur){
  //rotateRight in zig zig case at grandparent and zig zag at
  if(cur == NULL){
    return;
  }
  AVLNode<Key, Value>* temp = cur->getLeft();
  AVLNode<Key, Value>* p = cur->getParent();
  if(temp == NULL){
    return;
  }
  cur->setLeft(temp->getRight()); 
  if(temp->getRight()){
    temp->getRight()->setParent(cur);
  }
  if(p == NULL){ //zig zig
    BinarySearchTree<Key, Value>::root_ = temp;
  }
  else if(p->getLeft() == cur){
    p->setLeft(temp);
  }
  else if(p->getRight() == cur){
    p->setRight(temp);
  }
  temp->setParent(p);
  temp->setRight(cur);
  cur->setParent(temp);
  return;
}
template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* cur){
  //rotateLeft in zig zig case at grandparent
  if(cur == NULL){
    return;
  }
  AVLNode<Key, Value>* temp = cur->getRight();
  AVLNode<Key, Value>* p = cur->getParent();
  if(temp == NULL){
    return;
  }
  cur->setRight(temp->getLeft()); //zig zag case
  if(temp->getLeft()){
    temp->getLeft()->setParent(cur);
  }
  if(p == NULL){
    BinarySearchTree<Key, Value>::root_ = temp;
  }
  else if(p->getLeft() == cur){
    p->setLeft(temp);
  }
  else if(p->getRight() == cur){
    p->setRight(temp);
  }
  temp->setParent(p);
  cur->setParent(temp);
  temp->setLeft(cur);
  return;
}
/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
  //AVLNode<Key, Value>* n = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::internalFind(key));
  if(BinarySearchTree<Key, Value>::empty()){
    return;
  }
  AVLNode<Key, Value>* rem = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::internalFind(key));
  int8_t diff = 0;
  if(rem == NULL){
    return;
  }
  AVLNode<Key, Value>* original = rem->getParent();
  if(original == NULL && rem->getRight() == NULL && rem->getLeft() == NULL){
    delete rem;
    this->root_ = NULL;
    return;
  }
  //has 2 children
  if(rem->getRight() && rem->getLeft()){
    //swap with the predecessor
    nodeSwap(static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(rem)), rem);
  }
  AVLNode<Key, Value>* temp2 = rem->getParent();
  if(temp2){
    if(temp2->getLeft() == rem){
      diff = 1;
    } 
    //removing a right child
    else if(temp2->getRight() == rem){
      diff = -1;
    }
  }
  AVLNode<Key, Value>* temp1 = NULL;
  //has only the left child
  if(rem->getRight() == NULL && rem->getLeft()){ 
    //set to the left child
    temp1 = rem->getLeft();
  } 
  //has only the right child
  else if(rem->getRight() && rem->getLeft() == NULL){
    //set to the right child
    temp1 = rem->getRight();
  }
  AVLNode<Key, Value>* temp3 = rem->getParent();
  if(temp3){
    if(rem->getParent()->getLeft() == rem){
      temp3->setLeft(temp1);
    } 
    //removing a right child
    else if(rem->getParent()->getRight() == rem){
      temp2->setRight(temp1);
    }
  }
  else{
    this->root_ = temp1;
  }
  //promote the values
  if(temp1){
    temp1->setParent(temp2);
  }
  delete rem;
  removeFix(temp2, diff);
}
template<class Key, class Value>
void AVLTree<Key, Value>:: removeFix(AVLNode<Key, Value>* n, int8_t diff)
{
    // TODO
  if(n == NULL){
    return;
  }
  AVLNode<Key, Value>* p = n->getParent();
  int8_t ndiff = 0;
  if(p){
    if(p->getLeft() == n){
      ndiff = 1;
    }
    else{
      ndiff = -1;
    }
  }
  if(diff == -1){
    if((n->getBalance()+diff) == -2){
      AVLNode<Key, Value>* c = n->getLeft();
      //zig zig case
      if(c->getBalance() == -1){
        rotateRight(n);
        n->setBalance(0);
        c->setBalance(0);
        removeFix(p, ndiff);
      }
      else if(c->getBalance() == 0){
        rotateRight(n);
        n->setBalance(-1);
        c->setBalance(1);
      }
      else if(c->getBalance() == 1){
        //grandchild of n
        AVLNode<Key, Value>* g = c->getRight();
        rotateLeft(c);
        rotateRight(n);
        if(g->getBalance() == 1){
          n->setBalance(0);
          c->setBalance(-1);
          g->setBalance(0);
        }
        else if(g->getBalance() == 0){
          n->setBalance(0);
          c->setBalance(0);
          g->setBalance(0);
        }
        else if(g->getBalance() == -1){
          n->setBalance(1);
          c->setBalance(0);
          g->setBalance(0);
        }
        removeFix(p, ndiff);
      }
    }
    else if((n->getBalance()+diff) == -1){
      n->setBalance(-1);
    }
    else if((n->getBalance()+diff) == 0){
      n->setBalance(0);
      removeFix(p, ndiff);
    }
  }
  else if(diff == 1){
    if((n->getBalance()+diff) == 2){
      AVLNode<Key, Value>* c = n->getRight();
      //zig zig case
      if(c->getBalance() == 1){
        rotateLeft(n);
        n->setBalance(0);
        c->setBalance(0);
        removeFix(p, ndiff);
      }
      else if(c->getBalance() == 0){
        rotateLeft(n);
        n->setBalance(1);
        c->setBalance(-1);
      }
      else if(c->getBalance() == -1){
        //grandchild of n
        AVLNode<Key, Value>* g = c->getLeft();
        rotateRight(c);
        rotateLeft(n);
        if(g->getBalance() == -1){
          n->setBalance(0);
          c->setBalance(1);
          g->setBalance(0);
        }
        else if(g->getBalance() == 0){
          n->setBalance(0);
          c->setBalance(0);
          g->setBalance(0);
        }
        else if(g->getBalance() == 1){
          n->setBalance(-1);
          c->setBalance(0);
          g->setBalance(0);
        }
        removeFix(p, ndiff);
      }
    }
    else if((n->getBalance()+diff) == 1){
      n->setBalance(1);
    }
    else if((n->getBalance()+diff) == 0){
      n->setBalance(0);
      removeFix(p, ndiff);
    }
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
