#include "equal-paths.h"
#include <algorithm>
using namespace std;


// You may add any prototypes of helper functions here
int height(Node * root){
  if(root == nullptr){
    return 0;
  }
  return 1 + max(height(root->left), height(root->right));
}

bool equalPaths(Node * root)
{
    // Add your code below
    //|| root->left == nullptr || root->right == nullptr
  if(root == nullptr ){ //have reached a child node
    return true;
  }
  if(root->left != nullptr && root->right != nullptr){
    return height(root->right) == height(root->left);
  }
  if(root->left != nullptr && root->right == nullptr){
    return equalPaths(root->left);
  }
  if(root->left == nullptr && root->right != nullptr){
    return equalPaths(root->right);
  }
  if(root->left == nullptr && root->right == nullptr){
    return true;
  }
  return NULL;
  // int left_size = height(root->left);
  // int right_size = height(root->right);
  // if(left_size != 0 && right_size != 0){
  //   return right_size == left_size;
  // }

  // else if(height(root->left) == height(root->right)){
  //   //return equalPaths(root->left) && equalPaths(root->right);
  //   return true;
  // }

}

