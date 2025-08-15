#ifndef WAVELET_TREE_H__
#define WAVELET_TREE_H__

#include <vector>
#include <string>
#include <cstddef> 

using namespace std;

// wavelet_node is the node structure for the Wavelet Tree.
// Each node stores the value range it covers and a bitvector that determines whether each symbol goes to the left or right subtree.
struct wavelet_node {
  int lo;                     // Minimum value in this node's range
  int hi;                     // Maximum value in this node's range
  vector<int> bit_prefix;    
  wavelet_node* left;         // Left child
  wavelet_node* right;        // Right child
};

// Wavelet Tree:
//
// Compact data structure supporting:
//   - Access(i): Get the symbol at position i
//   - Rank(x, i): Count occurrences of x in positions [0, i)
//   - Select(c, k): Find position of the k-th occurrence of c
//
// This version is intended for compressing and querying sequences
// of document IDs or words from text collections.
class WaveletTree {
public:
  // Constructor: sets root to nullptr
  WaveletTree();

  // Destructor: cleans up allocated memory for the tree
  ~WaveletTree();

  // Build the wavelet tree from a set of integers, given the max and min values

  void build(const vector<int>& data, int min_val, int max_val);

  // Access: return the symbol at position pos (0-indexed).
  int access(wavelet_node* node, size_t pos);

  // Rank: count occurrences of value in positions [0, pos).
  int rank(wavelet_node* node, int value, size_t pos);

  // Select: return the position of the k-th occurrence of value.
  // k is 0-indexed (k=0 means first occurrence)
  int select(int value, int k);

  // Get root pointer
  wavelet_node* get_root();

  // Set root pointer
  void set_root(wavelet_node** new_root);

private:
  // Pointer to the root pointer of the wavelet tree
  wavelet_node* root;

  // Initialize a node covering the given value range
  wavelet_node* init_node(int lo, int hi);

  // Recursive helper to build the tree
  void build_recursive(wavelet_node* node, const vector<int>& data);

  // Recursive helper to delete the tree
  void delete_tree(wavelet_node* node);

  int select_recursive(wavelet_node* node, int value, int k) const;
};

#endif // WAVELET_TREE_H__
