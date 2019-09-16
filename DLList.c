// DLList.c - Implementation of doubly-linked list ADT
// Written by John Shepherd, March 2013
// Modified by Luka Gamulin, August 2019
// Luka Gamulin 
#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include "DLList.h"

char *strdup(const char *s);

// data structures representing DLList

typedef struct DLListNode {
	char *value;        /**< value of this list item (string) */
	struct DLListNode *prev;
	int numwords;                    /**< pointer to previous node in list */
	struct DLListNode *next;
	                    /**< pointer to next node in list */
} DLListNode;

typedef struct DLListRep {
	size_t nitems;      /**< count of items in list */
	DLListNode *first;  /**< first node in list */
	DLListNode *curr;   /**< current node in list */
	DLListNode *last;   /**< last node in list */
} DLListRep;


/** Function prototypes for static functions local to this file */
static DLListNode *newDLListNode(char *it);
static void freeDLListNode(DLListNode *node);

/** Create a new, empty DLList. */
DLList newDLList (void)
{
	DLListRep *new = malloc (sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate DLList");
	new->curr = new->first = new->last = NULL;
	new->nitems = 0;
	return new;
}

/** Release all resources associated with a DLList. */
void freeDLList (DLList L)
{
	if (L == NULL) return;

	DLListNode *curr = L->first;
	while (curr != NULL) {
		DLListNode *next = curr->next;
		freeDLListNode (curr);
		curr = next;
	}
	free (L);
}

/** Create a new DLListNode (private function) */
static DLListNode *newDLListNode (char *it)
{
	//Allocates space in heap for a new node
	DLListNode *new = malloc (sizeof *new);
	//checks that new is not NULL before accessing memory to allocate value
	if (new == NULL) err (EX_OSERR, "couldn't allocate DLList node");
	// Then given the input string creates an independent copy into the node.
	new->value = strdup(it);
	//by default prev and node will point at null
	new->prev = new->next = NULL;
    new->numwords = 1;
	return new;
}

/** Release a DLListNode (private function) */
static void freeDLListNode (DLListNode *node)
{
	if (node == NULL) return;
	free (node->value);
	free (node);
}


/** Return the item at current position. */
char *DLListCurrent (DLList L)
{
	assert (L != NULL);
	return L->curr != NULL ? L->curr->value : NULL;
}




//Written by Luka Gamulin

/** insert an item before current item
 * new item becomes current item */
void DLListBefore (DLList L, char *it)
{
	assert (L != NULL);
	//empty List
	DLListNode *new = newDLListNode(it);
	if (L->curr == NULL) {
		L->first = new;
		L->last = new;
		L->curr = new;
	}
	//curr is in head
	//condition can also be if curr == first
	else if (L->curr->prev ==  NULL) {
		new->next = L->first;
		L->first->prev = new;
		L->first = new;
		L->curr = new;
	}
	//curr is in last and curr is anywhere else in middle
	else {
		new->prev = L->curr->prev;
		L->curr->prev->next = new;
		L->curr->prev = new;
		new->next = L->curr;
		L->curr = new;
	}
	L->nitems++;
}



/** delete current item
 * new item becomes item following current
 * if current was last, current becomes new last
 * if current was only item, current becomes null */
void DLListDelete (DLList L)
{
	assert (L != NULL);
	//Empty List
	if (L->curr == NULL)
		return;
	//One node 
	DLListNode *temp = L->curr;
	if (L->curr->prev == NULL && L->curr->next == NULL) {
		L->curr = NULL;
		L->first = NULL;
		L->last = NULL;
		freeDLListNode(temp);
	}
	//curr is last 
	else if (L->curr == L->last && L->curr->prev != NULL) {
		L->curr->prev->next = NULL;
		L->curr = L->curr->prev;
		L->last = L->curr;
		freeDLListNode(temp);
	}
	//curr is head
	else if (L->curr == L->first && L->curr->next != NULL) {
		L->curr->next->prev = NULL;
		L->first = L->first->next;
		L->curr = L->first;
		freeDLListNode(temp);
	}
	//middle case
	else {
		L->curr->prev->next = L->curr->next;
		L->curr->next->prev = L->curr->prev;
		L->curr = L->curr->next;
		freeDLListNode(temp);
	}
	L->nitems--;
}


/** is the list empty? */
bool DLListIsEmpty (DLList L)
{
	return L->nitems == 0;
}


bool InList(DLList L, char * word) {
    assert(L != NULL);
    if(DLListIsEmpty(L)) return 0;
    int flag = 0;
    DLListNode *curr = L->first;
    while(curr != NULL) {
        if(strcmp(curr->value,word) == 0){
            curr->numwords++;
            flag = 1;
        }
        curr = curr->next;
    }
    return flag;
}
//Get current node num words
int GetWords(DLList L) {
    return L->curr->numwords;
}