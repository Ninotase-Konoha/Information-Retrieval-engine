//Luka Gamulin z5163726
#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "invertedIndex.h"
#include <ctype.h>
#include <sysexits.h>
#include "List.h"


//Compiling hack
char *strdup(const char *s);

//Create new FileList Node
FileList newFileListNode(char *file, double tfnum)
{
	FileList new = malloc (sizeof(struct FileListNode));
	if (new == NULL) err (EX_OSERR, "couldn't allocate Node");
	new->filename = strdup(file);
    new->tf = tfnum;
    new->next = NULL;
	return new;
}

//Given FileList show all elements associated, if list is empty do nothing
void showFileList(FILE *out ,FileList file) 
{
    if(file != NULL) {
        //if list is not empty print until curr is null
        FileList curr = file;
        while(curr != NULL) {
            fprintf(out,"%s ",curr->filename);
            curr = curr->next;
        }
    }
}

//Given a linked list counts the number of items in the list, return 0 if list is empty.
double countFiles(FileList fileList) 
{
    double count = 0;
    if(fileList == NULL) return count;
    //List is not empty
    FileList curr = fileList;
    while(curr != NULL) {
        count++;
        curr = curr->next;
    }
    return count;
}
//Given a linked list inserts the node in alphabetical order assumes files cannot be repeated
FileList InsertAlphaFile(FileList fileCollection, FileList newfile) 
{
    //empty list
    if(fileCollection == NULL) {
        return newfile;
    }//Else file contains nodes
    FileList curr = fileCollection;
    //Insert at head case
    if(strcmp(newfile->filename,curr->filename) < 0) {
        newfile->next = curr;
        return newfile;
    }

    //Middle case
    while(curr->next != NULL) {
        //if file is less than curr insert before
        if(strcmp(newfile->filename,curr->next->filename) < 0) {
            newfile->next = curr->next;
            curr->next = newfile;
            return fileCollection;
        }
        curr= curr->next;
    }
    //if it reaches here then insert at tail
    curr->next = newfile;
    return fileCollection;
}

//returns true if element in list
bool InFileList(FileList L, char * filename)
{
    assert(L != NULL);
    int flag = 0;
    FileList curr = L;
    while(curr != NULL) {
        if(strcmp(curr->filename,filename) == 0){
            flag = 1;
            break;
        }
        curr = curr->next;
    }
    return flag;
}



//Create new FileList Node
TfIdfList newTfIdfNode(char *file, double tfidfnum)
{
    //printf("file:%s,tfidfnum:%lf\n",file, tfidfnum);
	TfIdfList new = malloc (sizeof(struct TfIdfNode));
	if (new == NULL) err (EX_OSERR, "couldn't allocate Node");
	new->filename = strdup(file);
    new->tfidf_sum = (tfidfnum);
    new->next = NULL;
	return new;
}

//Given a File List, idf, and a TfIdfList returns TfIdf in ascending order with alpha conditons
TfIdfList getListTfIdf(FileList files, double idf, TfIdfList newList)
{
    //Nothing to do if word was not found
    if(files == NULL) return newList;
    //initialize list to be first element in FileList
    FileList curr = files;
    //condition is for next element in list not head
    while(curr != NULL) {
        //If current file name is not in list insert otherwise adds tfidf sum
        if(!InTfidfList(newList,curr,idf)) {
            newList = insertAscending(newList,curr, (curr->tf)*(idf));
        }
        curr = curr->next;
    }

    return newList;
}

//Given an existing TfIdfList inserts a file in ascending order relevant to tfidf sum
TfIdfList insertAscending(TfIdfList tfidflist , FileList node, double tfidf) 
{

    TfIdfList curr = tfidflist;
    TfIdfList new = newTfIdfNode(node->filename,(tfidf));
    //Empty List
    if(tfidflist == NULL) return new;

    //Check head case
    if((curr->tfidf_sum) < (tfidf)) {
        new->next = curr;
        return new;
    }

    //Middle case
    while(curr->next != NULL) {
        if((curr->next->tfidf_sum) < (tfidf)) {
            new->next = curr->next;
            curr->next = new;
            break;
        }
        curr = curr->next;
    }
    //Insert Tail case includes equal files
    curr->next = new;
    return tfidflist;
}


void printTfIdfListd(TfIdfList tfidflist)
{
    assert(tfidflist != NULL);
    TfIdfList curr = tfidflist;
    while(curr != NULL) {
        printf("tfidfsum:%lf, filename: %s ->", curr->tfidf_sum, curr->filename);
        curr = curr->next;
    }
    printf("\n");
}

//returns true if element in list if it is adds tfidf sum
bool InTfidfList(TfIdfList L, FileList node, double idf)
{


    //check if item is in list and add sum if true
    int flag = 0;
    TfIdfList curr = L;
    while(curr != NULL) {
        if((strcmp(curr->filename,node->filename) == 0)){
            //Recalculates the tfidf sum , deletes curr and reinserts the new node
            double tfidfsum = (curr->tfidf_sum) + ((node->tf) * (idf));
            L = DeleteTfIdf(L,node->filename);
            L = insertAscending(L,node,tfidfsum);
            flag = 1;
            break;
        }
        curr = curr->next;
    }
    return flag;
}

//Given a TfdIdfList deletes curr node
TfIdfList DeleteTfIdf(TfIdfList List, char *fileToDelete) 
{

    //Empty case
    if(List == NULL) return NULL;
   
    // Head case
    TfIdfList curr = List;
    if(strcmp(fileToDelete,curr->filename) == 0) {
        //One node case
        if(curr->next == NULL) {
            free(List);
            return NULL;
        }
        //Else head case
        else {
            List = curr->next;
            struct TfIdfNode * node = curr;
            free(node);
            return List;
        }
    }
    
    //Else traverse list and keep track of prev
    TfIdfList prev = curr;
    while(curr != NULL && (strcmp(fileToDelete,curr->filename) != 0)) {
        prev= curr;
        curr = curr->next;

    }

    // file not found
    if (curr == NULL) return List;
    struct TfIdfNode * node = curr; 
    // Unlink node
    prev->next = curr->next; 
    
    free(node);  // Free memory 
    return List;
} 
