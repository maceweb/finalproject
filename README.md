# finalproject
WAVELET TREES

In my project, I designed and implemented a Wavelet Tree based index that can efficiently store and query large datasets.

A wavelet tree is a binary tree data structure used for compact storage and the easy ability to query on text files. In my program I use three main functions:

Access(i) – retrieves the symbol at position i in the sequence
Rank(x, i) – counts how many times symbol x appears up to index i
Select(c, k) – finds the position of the k-th occurrence of symbol c
Background

The program will:

Process a text file and store the sequence in a wavelet tree.
By storing the postings in a wavelet tree, I can run efficient searches due to the reduced storage footprint.


Using my program, you will be able to accomplish the following:

Reads a text file and processes its contents
Compresses the sequence of words, or document IDs, into a binary wavelet tree structure
Can correctly run access, rank, and select operations directly on the compressed data
Returns correct results for queries without decompressing the entire file

For a full demostration and reflection please view my Google Doc Link here:https://docs.google.com/document/d/1pcbhECpjk8XKufYu8prrcvX1NmuxEh7AhuUppD2NtMk/edit?usp=sharing.