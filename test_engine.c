/** 

   Simple tests 

   1) copy (link) example data files and test_engine.c to a new dir, say "engine"
   2) cd to say "engine"
   3) copy files from repo to "engine"
   4) Generate executable using  the following command
      % gcc -Wall -Werror -lm -std=c11 *.c  -o  engine
   5) Run
      %  ./engine
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "invertedIndex.h" 

/** Util function below ...
*/
void printTfIdfList(char *filename, TfIdfList list){
	
	FILE *fp = fopen(filename, "w");
	if( fp == NULL ) { 
		printf("Error opening file : %s \n", filename );
		return;
	}

	TfIdfList cur = list;
	while(cur != NULL) {
		fprintf(fp, "%.6f  %s\n",  cur->tfidf_sum, cur->filename );
		cur = cur->next;
	}
	fclose(fp);
}


void checkNormalisedString(char *origString, char *answerString){

	char originalString[100];
	char *stuString;

	strcpy(originalString, origString);
	stuString = normaliseWord(originalString);

	if( (originalString == stuString) && 
	    (strcmp(stuString, answerString) == 0) ){
		printf("> Test Passed: %s to %s\n", origString, stuString);
	}
	else {
		printf("> Test Failed: %s to %s [Expected %s]\n", origString, stuString, answerString);
	}	
}


void testNormalise(){
	printf("Testing function  normaliseWord \n");
	checkNormalisedString(".Net", ".net");
	checkNormalisedString("smh.com.au", "smh.com.au");
	checkNormalisedString("Sydney!", "sydney!");
	checkNormalisedString("wHy?", "why");
	checkNormalisedString("ORDER.", "order");
	checkNormalisedString("Text;", "text");
	checkNormalisedString("abc.net.au.", "abc.net.au");
	checkNormalisedString("Sydney???", "sydney??");
}


int main (int argc, char *argv[]) {

	// =========   Part-1 Testing  =========  

	testNormalise(); 

	// ---------------------------------------------------------

	InvertedIndexBST invertedTree =  generateInvertedIndex("collection.txt");

	/** Your output in "invertedIndex.txt" should be 
	    same as the expected answer in "invertedIndex_exp.txt"
	*/
        printInvertedIndex(invertedTree); 


	// =========   Part-2 Testing =========  


	TfIdfList list = calculateTfIdf(invertedTree, "mars" , 7); 

	/** Your output in "mars_TfIdfList.txt" should be 
	    same as the expected answer in "mars_TfIdfList_exp.txt"
	*/
	printTfIdfList("mars_TfIdfList.txt" , list);



	TfIdfList list_sun = calculateTfIdf(invertedTree, "sun" , 7); 

	/** Your output in "sun_TfIdfList.txt" should be 
	    same as the expected answer in "sun_TfIdfList_exp.txt"
	*/
	printTfIdfList("sun_TfIdfList.txt" , list_sun);


	TfIdfList list_moon = calculateTfIdf(invertedTree, "moon" , 7); 
	printTfIdfList("moon_TfIdfList.txt" , list_moon );
		
	// ---------------------------------------------------------


	/**  -----  The following will be available over the weekend -----
	*/

	char *words[] = { "nasa", "mars", "moon", NULL }; 
	TfIdfList listM = retrieve(invertedTree, words , 7);

	/** Your output in "nasa_mars_moon.txt" should be 
	    same as the expected answer in "nasa_mars_moon.txt_exp.txt"
	*/
	printTfIdfList("nasa_mars_moon.txt" , listM);

	/*

		A "free" function to free "invertedTree" and the related memory,
		and call it here. The function is not currently developed.
	*/

	return 0;

}


