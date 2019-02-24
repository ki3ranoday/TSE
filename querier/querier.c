#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include "counters.h"
#include "index.h"
#include "readlinep.h"
#include "word.h"
#include "bag.h"
#include "pagedir.h"

/*
	holds a docID and a score for sorting;
*/
typedef struct docscore{
	int docID;
	int score;
} docscore_t;

/*** Function Declarations ***/
void run(const char* pagedir, index_t* idx, bag_t* cleanupbag);
int checkArgs(int argc, char const *argv[]);
bool goodQuery(char* queryString);
counters_t* query(index_t* idx, char* queryString, bag_t* cleanupbag);
counters_t* queryWord(index_t* idx, char* word, bag_t* cleanupbag);
int min(int one, int two);
void andMerge(counters_t* one, counters_t* two);
void counters_intersector(void *arg, const int key, int count);
void orMerge(counters_t* one, counters_t* two);
void counters_unioner(void *arg, const int key, int count);
char** parseWords(char* queryString, int numWords);
bool onlyAlpha(char* queryString);
int numWordsInQuery(char* queryString);
counters_t* counters_copy(counters_t* c);
void copyhelper(void* arg, const int key, int count);
void bag_deletehelper(void* item){
	counters_delete(item);
}
void sort_helper(void* arg, const int key, int count);
int counters_size(counters_t* c);
void size_helper(void* arg, const int key, int count);
docscore_t** sortdocs(counters_t* c, int size);



/*** MAIN ***/
int main(int argc, char const *argv[]){
	//check parameters
	int error = checkArgs(argc, argv);
	if(error != 0)
		return error;
	//creates index from parameters
	FILE* fp = fopen(argv[2],"r");
	index_t* idx = index_load(fp, 1000);
	fclose(fp);
	if(idx == NULL){
		fprintf(stderr, "Failed to load index\n");
		return 3;
	}
	//make a bag that all the counters you create will just be added to and then you can delete them all at once at the end
	bag_t* cleanupbag = bag_new();
	//get queries from the user and prints the results
	run(argv[1], idx, cleanupbag);
	//cleanup the memory
	bag_delete(cleanupbag, bag_deletehelper);
	index_delete(idx);
	return 0;
}
void run(const char* pagedir, index_t* idx, bag_t* cleanupbag){
	char* queryString;
	while((queryString = readlinep()) != NULL){
		if(goodQuery(queryString)){ //checks the query
			counters_t* result = query(idx, queryString, cleanupbag); //runs the query prints results kinda
			int resultsSize = counters_size(result);
			printf("%d documents match your query\n", resultsSize);
			docscore_t** results = sortdocs (result, resultsSize);
			for(int i = 0; i < resultsSize; i ++){
				webpage_t* page = loadpage(results[i]->docID, pagedir);
				printf("DocID:%d\tScore:%d\tURL: %s\n",results[i]->docID,results[i]->score, webpage_getURL(page));
				webpage_delete(page);
			}
			for(int i = 0; i < resultsSize; i ++)
				free(results[i]);
			free(results);
			printf("----------------------------\n");
		}
		free(queryString);
	}
}
/*** Function Definitions ***/
/*
	checks that there are the correct numbers of arguments
	checks that the page directory exists
	checks that the index file exists and is readable
*/
int checkArgs(int argc, char const *argv[]){
	if(argc != 3){
		fprintf(stderr, "Usage: ./querier pageDirectory indexFilename\n");
		return 1;
	}
	DIR* dirtest = opendir(argv[1]);
	if(dirtest != NULL){
		closedir(dirtest);
	} else {
		fprintf(stderr, "Failed to open page directory %s\n", argv[1]);
		return 2;
	}
	FILE* filetest = fopen(argv[2], "r");
	if(filetest != NULL){
		fclose(filetest);
	}else{
		fprintf(stderr, "Failed to open index file %s\n", argv[2]);
		return 3;
	}
	return 0;
}

/*
	checks if the query string is good:
	only alphabetical and spaces in the string
	no operators at beginning or end of string
	no two operators in a row
*/
bool goodQuery(char* queryString){
	//make sure the string only has
	if(!onlyAlpha(queryString)){
		fprintf(stderr, "Only use alphabetical characters and spaces in your query\n");
		return false;
	}
	//copy the query string so you don't modify it with parse words
	char queryCopy[strlen(queryString)+1];
	strcpy(queryCopy,queryString);

	int numWords = numWordsInQuery(queryCopy);
	char** words = parseWords(queryCopy, numWords);

	for (int i = 0; i < numWords; i ++){ //make all the words lowercase
		makelower(words[i]);
	}
	//check the first and last words to make sure they are not "and" or "or"
	if(strcmp(words[0], "and") == 0 || strcmp(words[0], "or") == 0){
		fprintf(stderr, "query cannot begin with an operator\n");
		free(words);
		return false;
	}
	if(strcmp(words[numWords-1], "and") == 0 || strcmp(words[numWords-1], "or") == 0){
		fprintf(stderr, "query cannot end with an operator\n");
		free(words);
		return false;
	}
	//check for consecutive operators
	bool lastWasOp = false;
	for(int i = 0; i < numWords; i ++){
		if(strcmp(words[i], "and") == 0 || strcmp(words[i], "or") == 0){
			if(lastWasOp){ //if you see a operator and the last one was also an operator, thats two in a row
				fprintf(stderr, "query cannot have two operators in a row\n");
				free(words);
				return false;
			}
			lastWasOp = true; //set lastwasop to true if you see an operator
		}else{
			lastWasOp = false; //set false if you see a non operator
		}
	}
	free(words);
	return true;
}

/*
	queries the index for the words
	takes in a queryString that is already checked
	uses the mergers to process the query string and create a final set of documents
*/
counters_t* query(index_t* idx, char* queryString, bag_t* cleanupbag){
	int numWords = numWordsInQuery(queryString);
	char** words = parseWords(queryString, numWords);
	counters_t* product = NULL;
	counters_t* sum = counters_new();
	bag_insert(cleanupbag,sum);
	// go through the words, if you see an OR, add the product to the sum, reset product as the query of the next word
	// if you see an AND, ignore it and go to the next word
	// each next word is andMerged with the product until you see an OR, then the product is orMerged to the sum
	// at the end, if the product has stuff in it, orMerge it with sum
	for (int i = 0; i < numWords; i ++){
		if(strcmp(words[i],"and") == 0) 
			continue; //skip ands; they are the same as two words just next to each other
		if(strcmp(words[i],"or") == 0){
			if(product != NULL){
				orMerge(sum, product);
				product = NULL;
			} //when you find an or, or merge the product with the sum and reset the product
			continue;
		}
		//if it gets here words[i] is not an operator
		if(product == NULL){ //if the product is NULL, just set the product to the query of this word
			product = queryWord(idx, words[i], cleanupbag);
		}else{ //otherwise get the union of this word and the product so far
			counters_t* nextWord = queryWord(idx, words[i], cleanupbag);
			andMerge(product, nextWord);
		}
	}
	//at the end orMerge the product and the sum
	if(product != NULL){
		orMerge(sum,product);
		product = NULL;
		//counters_delete(product);
	}
	free(words);
	return sum;
}

/*
	queries the index for one word
	really just findes the word in the hashtable and returns that word's counter
*/
counters_t* queryWord(index_t* idx, char* word, bag_t* cleanupbag){
	counters_t* copy = counters_copy(hashtable_find(idx, word));
	bag_insert(cleanupbag,copy);
	return copy;
}
/*
	this struct holds a pair of counters_t pointers
	it is a small struct that is useful to pass as an argument into counters_iterate
	so that the two structs can be compared
*/
typedef struct cpair {
	counters_t* one;
	counters_t* two;
} cpair_t;
/*
	very simple function to find the min of two ints
	returns the minimum of the two
*/
int min(int one, int two){
	if (one < two) return one;
	return two;
}
/*
	merges one and two based on intersection for the AND operator
	counters one will be the new merged set
	pages in both counters will have their count set to the min of the two
	pages not in both counters will have their count set to zero
*/
void andMerge(counters_t* one, counters_t* two){
	//cpair_t* pair = cpair_new(one,two);
	cpair_t pair = {one, two};
	counters_iterate(one, &pair, counters_intersector);
	//cpair_delete(pair);
}
/*
	arg holds the counter you are iterating over as one and the counter you are intersecting as two
	this does the itersection for one key
	if both counters have the key
	the first counter sets its count to the min of the counts
	if they dont both have the key
	the first counter sets its count to 0
*/
void counters_intersector(void *arg, const int key, int count){
	counters_t* one = ((cpair_t*)arg)->one;
	counters_t* two = ((cpair_t*)arg)->two;
	//if both sets have the word, choose the min and set it as the count for the key for set one
	if(counters_get(two, key) > 0){
		counters_set(one,key, min(count,counters_get(two, key)));
	}else{ //else set the key of set one to 0 (this is iterating through set 1 so it definitely has the key)
		//printf("key is %d count is %d counter is %p\n",key,count,(void*)one);
		int zero = 0;
		counters_set(one,key,zero);
	}
}

/*
	merges one and two based on union for the OR operator
	counters one will be the new merged set
	pages in both counters will have their count set to the sum of the two counters
	pages not in counters one will be added and their count will be set to their count in two
*/
void orMerge(counters_t* one, counters_t* two){
	counters_iterate(two, one, counters_unioner);
}
/*
	arg holds the two counters, you are iterating over two but setting changing one
	if one contains the same key as two, set one's count as the counts' sum
	if one does not contain the key, add the key to one and set its count to the count in two
*/
void counters_unioner(void *arg, const int key, int count){
	counters_t* one = (counters_t*)arg;
	counters_set(one,key, count + counters_get(one,key));
}
/*
	parses the query string into an array of words of size numwords, and returns it
	returns NULL if the query string has non alpha, non space characters
	returns NULL on any error
*/
char** parseWords(char* queryString, int numWords){
	if(!onlyAlpha(queryString)){ //make sure there are only spaces and alphabet chars in the string
		return NULL;
	}
	char** words = calloc(numWords,sizeof(char*)); //make an array that is the wordcount of the string
	int index = 0;
	//change the spaces directly after words into \0 characters
	char* wordptr = queryString;
	char* restptr = queryString;
	while(*wordptr != '\0'){ //if you have not reached the end of the string
		//slide the word pointer down unil you find a alphabetic character or hit the end char
		while(*wordptr != '\0' && !isalpha(*wordptr)){
			wordptr++;
		}
		if(*wordptr != '\0'){ //if the word ptr is the end char dont do this
			//save the wordptr in the words array
			words[index] = wordptr;
			index++; //increment the index
			if(index > numWords){
				fprintf(stderr, "numWords is too small\n");
				return NULL;
			}
			restptr = wordptr; //move the restptr to that spot
			while(isalpha(*restptr)){
				restptr++;
			} //move the restptr down until it is no longer an alpha char
			wordptr = restptr; //move the wordptr to the restptr
			if(*restptr != '\0'){ //if the ptr is not the end of the string
				*restptr = '\0'; //insert a terminating character for that word
				wordptr++; //slide the word ptr to the next char
			} //else the wordptr will still be '\0' so the while loop will terminate
		}
	}
	return words;
}
/*
	makes sure that every character in the string is either alpha or a space
	stops at the terminating char
*/
bool onlyAlpha(char* queryString){
	//check for invalid characters
	char* charptr = queryString;
	while(!(*charptr == '\0')){
		if(!isalpha(*charptr) && !isspace(*charptr)){
			fprintf(stderr, "%c is not a valid query character\n", *charptr);
			return false;
		}
		charptr++;
	}
	return true;
}
/*
	counts how many words are in a string
*/
int numWordsInQuery(char* queryString){
	int count = 0;
	if(!onlyAlpha(queryString)){
		return 0;
	}
	bool lastAlpha = false; //keeps track if the last letter I looked at was alphabetical
	for(int i = 0; i < strlen(queryString); i++){
		if(isalpha(queryString[i])){ //if this char is alpha
			if(!lastAlpha){ //and the last was not
				count++; //its a word!
				lastAlpha = true; //change last alpha to true
			} 
		}else{ //if the cur char is not alpha, change last alpha back to false
			lastAlpha = false;
		}
	}
	if(count == 0)
		fprintf(stderr, "QueryString has no words in it\n");
	return count;
}

counters_t* counters_copy(counters_t* c){
	if (c == NULL)
		return NULL;
	counters_t* copy = counters_new();
	counters_iterate(c, copy, copyhelper);
	return copy;
}
void copyhelper(void* arg, const int key, int count){
	counters_t* copy = (counters_t*)arg;
	counters_add(copy, key);
	counters_set(copy, key,count);
}

void size_helper(void* arg, const int key, int count){
	int* size = arg;
	if(count > 0)
		*size = *size + 1;
}
/*
counts the number of documents in a counter with score > 0
*/
int counters_size(counters_t* c){
	int size = 0;
	counters_iterate(c, &size, size_helper);
	return size;
}

typedef struct array{
	docscore_t** array;
	int* numinserted;
}array_t;

/*
	sorts the array of counters with score > 0
*/
void sort_helper(void* arg, const int key, int count){
	if(count > 0){
		array_t* a = (array_t*) arg;
		docscore_t** docs = a->array;
		docscore_t* ds = malloc(sizeof(docscore_t));
		ds->docID = key;
		ds->score = count;
		int startIndex = *(a->numinserted);
		docs[startIndex] = ds;
		//swap with any scores that are lower than the count by iterating backwards down the list
		for (int i = startIndex-1; i >= 0; i --){
			if(docs[i]->score < ds->score){
				docs[i+1] = docs[i];
				docs[i] = ds;
			}
		}
		*(a->numinserted) = *(a->numinserted)+1;
	}
}

/*
	sorts the counters with size > 0
*/
docscore_t** sortdocs(counters_t* c, int size){
	docscore_t** ds = malloc(size * sizeof(docscore_t*));
	int numin = 0;
	array_t docs = {ds, &numin};
	counters_iterate(c, &docs, sort_helper);
	return ds;
}







