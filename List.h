#ifndef LIST_H
#define LIST_H

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "invertedIndex.h"
#include "Tree.h"
#include <math.h>
#include <ctype.h>
#include <stdbool.h>

//Creates new file node
FileList newFileListNode(char *file, double tfnum);

//Given File to print, print all elements associated, if list is empty do nothing
void showFileList(FILE *out ,FileList file); 

//Counts the number of files in a FileList
double countFiles(FileList fileList);

//Given a linked list inserts the node in alphabetical order assumes files cannot be repeated
FileList InsertAlphaFile(FileList fileCollection, FileList newfile); 

//returns true if element in list
bool InFileList(FileList L, char * filename);

//Create new FileList Node
TfIdfList newTfIdfNode(char *file, double tfidfnum);

//Given a File List, idf, and a TfIdfList returns TfIdf in ascending order with alpha conditons
TfIdfList getListTfIdf(FileList files, double idf, TfIdfList newList);

//Given an existing TfIdfList inserts a file in ascending order relevant to tfidf sum
TfIdfList insertAscending(TfIdfList tfidflist , FileList node, double tfidf);

void printTfIdfList2(TfIdfList tfidflist);

//returns true if element in list if it is adds tfidf sum
bool InTfidfList(TfIdfList L, FileList node, double idf);

//Given a TfdIdfList deletes curr node
TfIdfList DeleteTfIdf(TfIdfList List, char *fileToDelete);

#endif
