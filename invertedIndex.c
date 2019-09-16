//Ass1 , Luka Gamulin z5163726
#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sysexits.h>
#include "DLList.h"
#include <math.h>
#include "Tree.h"
#include "List.h"
#include "invertedIndex.h"


//Given a string sets all the words to lowercase
static char * lowerCase(char *str) {    
    for(int i = 0; str[i]; i++)
        str[i] = tolower(str[i]);
    return str;
}

// Functions for Part-1
char * normaliseWord(char *str) {
    //check the string is not empty
    assert(str != NULL);
    //initialiaze counter
    int i = 0, len = strlen(str);
    //iterate through string and get chars, if the last char is: "." "?" ";" ":" "," do not include in string
    while(i < len) {
        if((str[i] == '.') || (str[i] == '?') || (str[i] == ',') 
            || (str[i] == ';') || (str[i] == ':'))
            if ((str[i+1] == '\0'))  
                break;
        i++;
    }
    str[i] = '\0';
    //Set all words to lower case
    str = lowerCase(str);
    
    return str;
}


InvertedIndexBST generateInvertedIndex(char *collectionFilename) 
{    
    FILE* ptr = fopen(collectionFilename,"r"); 
    if (ptr==NULL) { 
        printf("no such file."); 
    } 

    char buf[1000];

    InvertedIndexBST tree = NULL;
    while (fscanf(ptr,"%s",buf)==1) {
        //GenTree in Tree.c
        tree = GenTree(tree,buf);
    }
    return tree;
}

//Print words in ascending order with relevant files
void printInvertedIndex(InvertedIndexBST tree) 
{
   FILE *out;
   out = fopen("invertedIndex.txt", "w");
   BSTreeInfix(tree,out);
   fclose(out);
}

//Calculate TfIdf if word not found returns NULL
TfIdfList calculateTfIdf(InvertedIndexBST tree, char *searchWord , int D) {
    assert(tree != NULL);
    FileList FilesRelated = getListTree(tree,searchWord);
    if(FilesRelated == NULL) return NULL; //word not found
    double idftemp = idf(FilesRelated,D);
    TfIdfList newList = NULL;
    newList = getListTfIdf(FilesRelated,idftemp,newList);
    return newList;

}

//Given an array of words retrieves relevant files
TfIdfList retrieve(InvertedIndexBST tree, char* searchWords[] , int D) {
    int i = 0;
    TfIdfList newList = NULL;
    FileList currFiles = NULL;
    while(searchWords[i] != NULL) {
        currFiles =  getListTree(tree,searchWords[i]); 
        double idftemp = idf(currFiles,D);
        newList = getListTfIdf(currFiles,idftemp,newList);
        i++;
    }
    return newList;
}

