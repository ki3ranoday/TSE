#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "counters.h"
#include "index.h"



int main(int argc, char const *argv[]){
	//check parameters
	int error = checkArgs(argc, argv);
	if(error != 0)
		return error;


	return 0;
}
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
no operators at beginning or end of string
no two operators in a row
no non alpha non space characters
*/
bool goodQuery(char* queryString){

}
/*
	queries the index for the words
	takes in a querystring that should be already normalized and lowercase
	uses the mergers to process the query string and create a final set of documents
*/
counters_t* query(index_t* idx, char* queryString){

}
/*
	queries the index for one word
	really just findes the word in the hashtable and returns that word's counter
*/
counters_t* queryWord(index_t* idx, char* word){
	return hashtable_find(idx, word);
}

/**
this struct holds a pair of counters_t pointers
it is a small struct that is useful to pass as an argument into counters_iterate
so that the two structs can be compared
*/
typedef struct cpair {
	counters_t* one;
	counters_t* two;
} cpair_t;
cpair_t* cpair_new(counters_t* one, counters_t* two){
	cpair_t* newpack;
	newpack->one = one;
	newpack->two = two;
	return newpack;
}
/**
	very simple function to find the min of two ints
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
void andMerger(counters_t* one, counters_t* two){
	counters_iterate(one, cpair_new(one, two), counters_intersector);
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
	//argument is a cpair_struct, has counters_t* one and two
	//if both sets have the word, choose the min and set it as the count for the key for set one
	if(counters_get(arg->one, key) > 0 && counters_get(arg->two, key) > 0){
		counters_set(arg->one, key, min(counters_get(arg->one, key),counters_get(arg->two, key)));
	}else{ //else set the key of set one to 0 (this is iterating through set 1 so it definitely has the key)
		counters_set(arg->one, key, 0);
	}
	free(arg);//free the cpair passed in, it is now useless, can free counters stored there later because their pointers are kept track of in a different place
}

/*
	merges one and two based on union for the OR operator
	counters one will be the new merged set
	pages in both counters will have their count set to the sum of the two counters
	pages not in counters one will be added and their count will be set to their count in two
*/
void orMerger(counters_t* one, counters_t* two){
	counters_iterate(two, cpair_new(one, two), counters_unioner);
}
/*
	arg holds the two counters, you are iterating over two but setting changing one
	if one contains the same key as two, set one's count as the counts' sum
	if one does not contain the key, add the key to one and set its count to the count in two
*/
void counters_unioner(void *arg, const int key, int count){
	//argument is a cpair_struct, has counters_t* one and two, count is two's count
	if(counters_get(arg->one, key) > 0){ //if the key is in the first counters
		counters_set(arg->one,key, counters_get(arg->one, key) + count); //set its value to the sum of it and count
	}else{ //otherwise add it and set its value to the count from the second counters
		counters_add(arg->one,key);
		counters_set(arg->one,count);
	}
	free(arg);//free the cpair passed in, it is now useless, can free counters stored there later because their pointers are kept track of in a different place
}
/*
	parses the query string into an array of words and returns it
*/
char** parseWords(char* queryString){

}











