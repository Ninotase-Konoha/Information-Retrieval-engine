#ifndef TREE_H
#define TREE_H

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "List.h"
#include "invertedIndex.h"
#include "DLList.h"
#include "List.h"
//BSTree functions from BSTree.c COMP2521
// A few modification made changing values for int to char string

//Create new tree
InvertedIndexBST newBSTNode (char *word);

//Get tf
double tf(int words, int totalwords);

//get idf
double idf(FileList currFiles, int D);

// insert a new value into a BSTree with relevant file
InvertedIndexBST BSTreeInsert (InvertedIndexBST t, char *word, FileList file);

//Shows root of the tree
void showBSTreeNode (InvertedIndexBST t,FILE *out);

//Prints tree in prefix order
void BSTreeInfix (InvertedIndexBST t, FILE *out);

//Generates a tree by scanning a file, input: tree and file string
//output: new tree if empty, if tree is not empty inserts words in existing tree
InvertedIndexBST GenTree(InvertedIndexBST tree,char *s);

// insert a new value into a BSTree with relevant file
InvertedIndexBST BSTreeInsert (InvertedIndexBST t, char *word, FileList file);

//search item in tree retrieve files in that node, returns NULL if node not found
FileList getListTree(InvertedIndexBST t, char *word);

#endif
