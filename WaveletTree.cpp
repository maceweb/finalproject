#include "WaveletTree.h"
#include <iostream>      // For debugging, if needed
#include <algorithm>     // For std::lower_bound
#include <stdexcept>     // For std::runtime_error

WaveletTree::WaveletTree() {
    root = nullptr;
}

WaveletTree::~WaveletTree() {
    delete_tree(root);
}

wavelet_node* WaveletTree::init_node(int lo, int hi) {
    wavelet_node* new_node = new wavelet_node;
    new_node->lo = lo;
    new_node->hi = hi;
    new_node->left = nullptr; 
    new_node->right = nullptr; 
    new_node->bit_prefix.clear(); 
    return new_node;
}

//Building Wavelet tree, starting with root node

void WaveletTree::build(const std::vector<int>& data, int min_val, int max_val) {

    if (min_val > max_val) {
        throw std::invalid_argument("Invalid value range: min_val cannot be greater than max_val.");
    }
    root = init_node(min_val, max_val);
    build_recursive(root, data);
}

void WaveletTree::build_recursive(wavelet_node* node, const std::vector<int>& data) {
    if (!node || data.empty() || node->lo == node->hi) {
        return;
    }

    //Find mid point to partition values in the tree
    int mid = node->lo + (node->hi - node->lo) / 2;

    node->bit_prefix.reserve(data.size() + 1);
    node->bit_prefix.push_back(0); 

    std::vector<int> left_data;  // Stores elements that go to the left child node
    std::vector<int> right_data; // Stores elements that go to the right child node

    // Iterate through the data for the current node, populate bit_prefix, and partition data
    for (size_t i = 0; i < data.size(); ++i) {
        if (data[i] <= mid) {
            left_data.push_back(data[i]);
            node->bit_prefix.push_back(node->bit_prefix.back() + 1);
        } else {
            right_data.push_back(data[i]);
            node->bit_prefix.push_back(node->bit_prefix.back());
        }
    }

    // Build left and right child nodes
    if (!left_data.empty()) {
        node->left = init_node(node->lo, mid); // Create left child node
        build_recursive(node->left, left_data); // Recurse for left child
    }
    if (!right_data.empty()) {
        node->right = init_node(mid + 1, node->hi); // Create right child node
        build_recursive(node->right, right_data);  // Recurse for right child
    }
}

//Access Function

int WaveletTree::access(wavelet_node* node, size_t pos) {
    if (!node) {
        return -1;
    }

    if (pos >= node->bit_prefix.size() - 1 && !node->bit_prefix.empty()) {
        return -1;
    }

    if (node->lo == node->hi) {
        return node->lo;
    }

    bool goes_left = (node->bit_prefix[pos + 1] - node->bit_prefix[pos] == 1);

    size_t new_pos; 

    if (goes_left) {

        new_pos = node->bit_prefix[pos];
        if (!node->left) {
            return -1;
        }
        return access(node->left, new_pos); 
    } else {

        new_pos = pos - node->bit_prefix[pos];
        if (!node->right) {
            return -1;
        }
        return access(node->right, new_pos); 
    }
}

//Rank Function

int WaveletTree::rank(wavelet_node* node, int value, size_t pos) {
    if (!node || pos == 0) {
        return 0;
    }

    if (pos > node->bit_prefix.size() - 1 && !node->bit_prefix.empty()) {
        return -1;
    }

    if (node->lo == node->hi) {
        return (node->lo == value) ? (int)pos : 0;
    }

    // Find mid point to partition values in the tree
    int mid = node->lo + (node->hi - node->lo) / 2;
    size_t new_pos; 

    if (value <= mid) {
        // If the `value` is in the left child's range, we only care about elements
        // that went left in the current node.
        if (!node->left) {
            return 0; // If no left child, no occurrences of value in this branch.
        }
        // The new position is the count of elements that went left up to `pos`.
        new_pos = node->bit_prefix[pos];
        return rank(node->left, value, new_pos); // Recurse on the left child
    } else {
        // If the `value` is in the right child's range, we only care about elements
        // that went right in the current node.
        if (!node->right) {
            return 0; // If no right child, no occurrences of value in this branch.
        }
        // The new position is the count of elements that went right up to `pos`.
        new_pos = pos - node->bit_prefix[pos];
        return rank(node->right, value, new_pos); // Recurse on the right child
    }
}

//Select Function

int WaveletTree::select(int value, int k) {
    if (!root || k < 0) return -1;
    return select_recursive(root, value, k); // call helper
}

// private helper (must be const as declared in header)
int WaveletTree::select_recursive(wavelet_node* node, int value, int k) const {
    if (!node) return -1;

    // Leaf node
    if (node->lo == node->hi) {
        return (node->lo == value) ? k : -1;
    }

    int mid = (node->lo + node->hi) / 2;

    if (value <= mid) {
        if (!node->left) return -1;

        int pos_in_left = select_recursive(node->left, value, k);
        if (pos_in_left == -1) return -1;

        int zeros_seen = 0;
        for (size_t i = 0; i < node->bit_prefix.size() - 1; ++i) {
            if (node->bit_prefix[i + 1] - node->bit_prefix[i] == 1) {
                if (zeros_seen == pos_in_left) return i;
                zeros_seen++;
            }
        }
    } else {
        if (!node->right) return -1;

        int pos_in_right = select_recursive(node->right, value, k);
        if (pos_in_right == -1) return -1;

        int ones_seen = 0;
        for (size_t i = 0; i < node->bit_prefix.size() - 1; ++i) {
            if (node->bit_prefix[i + 1] - node->bit_prefix[i] == 0) {
                if (ones_seen == pos_in_right) return i;
                ones_seen++;
            }
        }
    }

    return -1;
}



//Get Root function - returns the internal pointer

wavelet_node* WaveletTree::get_root() {
    return root;
}

//Set Root function

void WaveletTree::set_root(wavelet_node** new_root) {
    if (new_root) {
        this->root = *new_root; 
        *new_root = nullptr;     
    } else {
        this->root = nullptr; 
    }
}

//Delete Tree Function

void WaveletTree::delete_tree(wavelet_node* node) {
    if (node) {
        delete_tree(node->left); 
        delete_tree(node->right); 
        delete node;            
    }
}
