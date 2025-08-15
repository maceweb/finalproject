#include "WaveletTree.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

// Utility: read integers from a text file
vector<int> read_integers_from_file(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        exit(1);
    }

    vector<int> data;
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        int value;
        while (iss >> value) {
            data.push_back(value);
        }
    }
    return data;
}

int main() {
    // Step 1: Read data from file
    string filename;
    cout << "Enter filename: ";
    cin >> filename;

    vector<int> data = read_integers_from_file(filename);
    if (data.empty()) {
        cerr << "File is not readable" << endl;
        return 1;
    }

    // Step 2: Determine min and max values in data
    int min_val = data[0], max_val = data[0];
    for (int x : data) {
        if (x < min_val) min_val = x;
        if (x > max_val) max_val = x;
    }

    // Step 3: Build the Wavelet Tree
    WaveletTree wt;
    wt.build(data, min_val, max_val);

    cout << "Wavelet Tree built!!!" << endl;

    // Step 4: Test
    wavelet_node* root = wt.get_root();

    // Access Function
    size_t pos;
    cout << "Enter position to access: ";
    cin >> pos;
    if (pos < data.size()) {
        cout << "Value at position " << pos << ": " << wt.access(root, pos) << endl;
    } else {
        cout << "Invalid position!" << endl;
    }

    // Rank Function
    int val;
    cout << "Enter value that you would like to count: ";
    cin >> val;
    cout << "Enter position for rank: ";
    cin >> pos;
    if (pos <= data.size()) {
        cout << "Rank(" << val << ", " << pos << ") = " << wt.rank(root, val, pos) << endl;
    } else {
        cout << "Invalid position!" << endl;
    }

    // Select Function
    int k;
    cout << "Enter value for select: ";
    cin >> val;
    cout << "Enter occurrence index: ";
    cin >> k;
    int sel_pos = wt.select(val, k);
    if (sel_pos >= 0 && sel_pos < (int)data.size()) {
        cout << "Select(" << val << ", " << k << ") position = " << sel_pos << endl;
    } else {
        cout << "Occurrence not found!" << endl;
    }

    return 0;
}
