/*
  Class Implementation File
  In this file the functions are implemented.
*/
#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <assert.h>
#include "bstSequence.h"
using namespace std;
/*
  Fix Size Funtion
  This function takes a root node as a parameter and fixes the size of the node based on how many nodes
  are in the tree.
*/
void fix_size(Node *T)
{
  T->size = 1;
  if (T->left) T->size += T->left->size;
  if (T->right) T->size += T->right->size;
}
/*
  Insert Function
  This function takes a root node, a value and a rank as parameters and returns a node to the root of the tree
  Insertion is done by recursion until the desired rank is reached.
  The value does not affect the insertion process.
*/
Node *insert(Node *T, int v, int r)
{

  if(T == nullptr) return new Node(v); // If the tree is empty or an empty node is reached create a new node and return it.
  int rank_of_root = T->left ? T->left->size : 0; // Get the rank of the root
  if(r <= rank_of_root) T->left = insert(T->left,v,r); // If the rank is less than that of the root call the function again for the left side.
  else T->right = insert(T->right,v,r - rank_of_root - 1); // If the rank is bigger than that of the root call the function again for the right side.
  fix_size(T);// Fix the size
  return T;

}
/*
  In Order Traversal
  This function takes a root node as a parameter and returns a vector of elements in a sorted manner.
  This function is implemented using recursion.
*/
vector<int> inorder_traversal(Node *T)
{
  vector<int> inorder;
  vector<int> rhs;
  if (T == nullptr) return inorder; // If the end is reached or the tree is empty.
  inorder=inorder_traversal(T->left); // The left side is inserted into the vector.
  inorder.push_back(T->key); // The root value is inserted.
  rhs=inorder_traversal(T->right); // Right hand side is retrieved using recursion.
  inorder.insert(inorder.end(), rhs.begin(), rhs.end()); //Right hand side is inserted into the vector.
  return inorder; //Resulting vector is returned.
}
/*
  Select
  This function takes a root node and a rank as parameters and returns the selected node in the tree.
  Ranking is implemeted so that rank 0 is equal to the left most value in the tree.
*/
Node *select(Node *T, int r)
{
  assert(T!=nullptr && r>=0 && r<T->size); // Necessary conditions to implement the function is checked.

  int rank_of_root = T->left ? T->left->size : 0; // Rank of the root node is calculated.
  if (r == rank_of_root) return T; // If desired rank is equal to that of the root node's root node is returned.
  if (r < rank_of_root) return select(T->left, r); // If the desired rank is less than that of root node's the function is called for the left side of the tree.
  else return select(T->right, r - rank_of_root - 1); // If the desired rank is greater than that of root node's the function is called for the right side of the tree.
}
/*
  Split
  This function takes a root node, a rank, and two new root nodes.
  This function splits a tree in the rth rank into two new binary trees.
*/
void split(Node *T, int r, Node **L, Node **R)
{
  if(T != nullptr) { // If the tree is empty, the function does not do anything.
    int rank_of_root = T->left ? T->left->size : 0; // The rank of the root is calculated.
    if(r <= 0) { // If the rank is 0 or less the left side will be empty and the right side will be the entire tree.
      *R = T;
      *L = nullptr;
    }
    else if(r >= T->size){ //If the rank is size of the tree or bigger the right side will be empty and left side will be the entire tree.
      *R = nullptr;
      *L = T;
    }
    else if(r <= rank_of_root) { // If r is less than or equal to rank of the root set the right side to T and modify the left side of the right side tree by calling the split function again.
      Node* right_side_tree=T;
      if(T->left != nullptr) split(T->left,r,L,&right_side_tree->left);
      //Fix the sizes
      fix_size(right_side_tree);
      fix_size(*L);
      *R = right_side_tree;
    }
    else{// If r is more than rank of the root set the left side to T and modify the right side of the left tree by calling the split function again.
      Node *left_side_tree = T;
      if(T->right != nullptr) split(T->right,r - rank_of_root - 1,&left_side_tree->right,R);
      //Fix the sizes
      fix_size(left_side_tree);
      fix_size(*R);
      *L = left_side_tree;
    }
  }
}
/*
  Insert Random
  This function takes a root node, a value and a rank as parameters and returns a root node to the tree.
  This function is designed to keep the tree balanced.  
  If (v,r) is the Nth node inserted into T, then:
  with probability 1/N, insert (v,r) at the root of T
  otherwise, insert_random (v,r) recursively left or right of the root of T
*/
Node *insert_random(Node *T, int v, int r)
{

  if(T == nullptr) return new Node(v); // If tree is empty insert a new node
  int probability = rand() % T->size; // Return a random number within the range accepted
  int rank_of_root = T->left ? T->left->size : 0; // Find the rank of the root
  //1/N probability where N is the size 
  if(probability == 0) {
    Node *new_tree = new Node(v); // Create a new tree with key v
    split(T,r,&new_tree->left,&new_tree->right); // Split the original tree into two and set the resulting tree's left and right sides.
    fix_size(new_tree); //Fix the size
    return new_tree;
  }
  else if(r<=rank_of_root) T->left = insert_random(T->left,v,r); // If 1/N probability is not the case and the rank is less than or equal to the rank of the root recursively call the function again for left side of the tree
  else T->right = insert_random(T->right,v,r - rank_of_root - 1); // If 1/N probability is not the case and the rank is bigger than rank of the root recursively call the function again for right side of the tree
  fix_size(T); // Fix the size
  return T;
}
/*
  Did X Beat Y
  This function takes two integers representing two teams and returns a boolean based on whether the first team beat the second team or not.
  Returns true if team x defeated team y
*/
bool did_x_beat_y(int x, int y)
{
  assert (x != y); // The function only works if the teams are different than each other.
  if (x > y) return !did_x_beat_y(y,x);
  unsigned long long lx = x;
  unsigned long long ly = y;
  return ((17 + 8321813 * lx + 1861 * ly) % 1299827) % 2 == 0; // A random function to determine which team won.
}
/*
  Order N Teams
  This function takes an integer as a parameter and returns the root node of the binary tree consisting of n teams.
  With this order the ranking of all teams are determined.
*/
Node *order_n_teams(int n)
{
  Node *T = nullptr;

  // start by inserting the first team
  T = insert_random(T, 0, 0);

  // now insert the other teams...
  for (int i=1; i<n; i++) {
    // insert team i so the sequence encoded by the BST remains valid
    if (did_x_beat_y(i, select(T,0)->key)) // can we insert at beginning?
      T = insert_random(T, i, 0);
    else if (did_x_beat_y(select(T,T->size-1)->key, i)) // can we insert at end?
	    T = insert_random(T, i, T->size);
    else {
      //Below the interval of ranks to be searched is determined
      int high = T->size-1; 
      int low = 1;
      int rank_to_place;
      int tester;
      //Binary Search Modified
      while(high!=low) { // While high is not equal to the low
        tester = (high + low) /2; // Tester is the middle point to be tested.
        // If the tester team beat the ith team and ith team beat the team that comes directly after the tester team the rank to place is tester + 1 and the loop ends
        if(did_x_beat_y(select(T,tester)->key,i) && did_x_beat_y(i,select(T,tester+1)->key)) {rank_to_place = tester+1; high = low;}
        // If the tester team beat the ith team but ith team didnt beat the team after the tester team
        else if(did_x_beat_y(select(T,tester)->key,i)) low = tester+1;
        // If the tester team lost to the ith team
        else high = tester - 1;
      }
      // This while loop omits one case where low and high are equal to each other
      // And the test for it is implemented below
      if((did_x_beat_y(select(T,high)->key,i) && did_x_beat_y(i,select(T,high+1)->key))) rank_to_place = high+1;
      T = insert_random(T,i,rank_to_place); // Ith team is inserted into the tree
    }
  }
  return T;
}
/*
  Print Vector
  This function takes a vector as a parameter and prints the elements of it.
*/
void printVector(vector<int> v)
{
    for (int i=0; i<v.size(); i++)
    {
        cout << v[i] << endl;
    }
}
