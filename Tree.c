//Ass1 , Luka Gamulin z5163726
#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "invertedIndex.h"
#include <ctype.h>
#include <sysexits.h>
#include "DLList.h"
#include "List.h"

//To compile with -Werror
char *strdup(const char *s);
//Create new tree
InvertedIndexBST newBSTNode (char *word)
{
	InvertedIndexBST new = malloc (sizeof(struct InvertedIndexNode));
	if (new == NULL) err (EX_OSERR, "couldn't allocate BST node");
    //independent copy of string
	new->word = strdup (word);
	new->left = new->right = NULL;
    new->fileList = NULL;
	return new;
}

//Get tf
double tf(int words, int totalwords) 
{
    double tf = (double)words/totalwords;
    //printf("tf:%f, words:%d, totalwords:%d\n",tf,words,totalwords);
    return tf;
}
//Get idf 
double idf(FileList currFiles, int D) {
    double filesInword = countFiles(currFiles);
    double x = (filesInword/D);
    double idf = log10(x);
    idf = fabs(idf);
    return idf;
}

//Shows root of the tree
void showBSTreeNode (InvertedIndexBST t,FILE *out)
{
	if (t == NULL) return;
	fprintf (out,"%s ", t->word);
    showFileList(out,t->fileList);
    fprintf(out,"\n");
}


//Prints tree in prefix order
void BSTreeInfix (InvertedIndexBST t, FILE *out)
{
	if (t == NULL) return;

    BSTreeInfix (t->left,out);
	showBSTreeNode (t,out);
    BSTreeInfix (t->right,out);
}


//Generates a tree by scanning a file, input: tree and file string
//output: new tree if empty, if tree is not empty inserts words in existing tree
InvertedIndexBST GenTree(InvertedIndexBST tree,char *s) {
    FILE* ptr = fopen(s,"r");
    if (ptr==NULL) {
        printf("no such file.");
    }
    char buf[1000];
    //Create doubly link list for word handling
    DLList dllist = newDLList();
    //'i' will be the total num of words in that file
    int i = 0;
    //word is a pointer char to normalise words in file
    char *word;
    //Puts all the words of a file in a doubly linked list to handle
    //words in one go instead of looping multiple times
    while (fscanf(ptr,"%s",buf)==1) {
        //normalises word in file
        word = normaliseWord(buf);
        //Check word is not in list for insertion.
        //If its in list InList increments the word counter in the node
        if(!InList(dllist,word))
            DLListBefore(dllist,word);
        i++;
    }
    //All words of a file are now in the doubly linked list
    while(!DLListIsEmpty(dllist)) {
        //gets the tf from total words and num of words in each node
        double tftemp = tf(GetWords(dllist),i);
        FileList currfile = newFileListNode(s,tftemp);

        //inserts word in tree with current file in alpha order
        tree = BSTreeInsert(tree,DLListCurrent(dllist),currfile);
        DLListDelete(dllist);
    }
    freeDLList(dllist);
    fclose(ptr);

    return tree;
}


// insert a new value into a BSTree with relevant file
InvertedIndexBST BSTreeInsert (InvertedIndexBST t, char *word, FileList file)
{

	if (t == NULL) {
        t = newBSTNode(word);
        t->fileList = InsertAlphaFile(t->fileList, file);
        //printf("====TREE:===\n t->word:%s\n t->file: %s\n",t->word,file->filename);
		return t;
    }
    //If root is more than word send word to left
    else if ((strcmp(t->word,word) > 0)) {
		t->left = BSTreeInsert (t->left, word, file);
    //If root is smaller than word send word to right
	}
    else if ((strcmp(t->word,word) < 0)) {
		t->right = BSTreeInsert (t->right, word, file);
    }
    else { // (same word then insert the file in the list)
		//This will only happen for multiple files;
        t->fileList = InsertAlphaFile(t->fileList,file);
        return t;

    }
    return t;
}



//search item in tree retrieve files in that node, returns NULL if node not found
FileList getListTree(InvertedIndexBST t, char *word)
{
    if(t == NULL){ // word not found
        return NULL;
    }
    else if (strcmp(t->word, word) > 0) // send to the left
        return getListTree(t->left,word);
    else if(strcmp(t->word, word) < 0) // send to the right
        return getListTree(t->right,word);
    else {
        //word found check number of files in word
        return t->fileList;
    }
}