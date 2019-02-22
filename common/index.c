#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hashtable.h"
#include "counters.h"
#include "readlinep.h"



typedef hashtable_t index_t;

/**
create a new empty hashtable, return null if error
*/
index_t* index_new(int num_slots){
	return hashtable_new(num_slots);
}


/*
insert a word from a document into the index
the word is the key for the hashtable,
insert new counter if word not yet in table
the counter has all the docIds the word is in and their counts
return false if failed to insert
*/
bool index_insert(index_t* index, char* word, int docID){
	if (hashtable_find (index, word) == NULL){
		hashtable_insert(index, word, counters_new()); //insert a new counters at word in hashtable
	}
	if(counters_add(hashtable_find (index, word), docID) > 0){//counters added it successfully
		return true;
	}
	return false;
}

/*
return the count of the word in the document
return less than 1 if any error(count of a word can't be zero because when you add it the first time it is 1)
*/
int index_find(index_t* index, char* word, int docID){
	if(hashtable_find(index, word) == NULL) //if the word is in the hashtable
		return 0;
	//return the count of words in docID or <1 for error
	return counters_get(hashtable_find(index, word), docID); 
}


/*
helper function for index_print (just calls the counters print method)
*/
void counters_printer(FILE *fp, const char *key, void *item){ //formatted so it can be take by hashtable print
	if(item!=NULL && key!= NULL)
		counters_print(item,fp); // call the print function on the counter set
}
/*
print out the hashtable by printing each counter use the print functions for each
*/
void index_print(index_t* index, FILE* fp){
	if(index != NULL)
		hashtable_print(index, fp, counters_printer);
}


void counters_deleter(void* item){
	counters_delete(item);
}
/* Delete the whole index; ignore NULL ht.
 */
void index_delete(index_t* index){
	hashtable_delete(index, counters_deleter);
}


/*
passed into the counter iterator function in the counters saver
prints each key and each count like this:
key1 count1 key2 count2 ...
*/
void counters_saver_helper(void *fp, const int key, int count){
	fprintf((FILE*)fp, "%d %d ", key, count);
}
/*
prints a counter in the format needed for saving it:
word docID count docID count ....
*/
void counters_saver(void* fp, const char *key, void *item){
	fprintf((FILE*)fp, "%s ", key); //print the key and a space
	counters_iterate(item,fp,counters_saver_helper); //prints all the docIDs and counts
	fprintf((FILE*)fp, "\n"); //prints a new line
}
/*
prints the index to a file so it can be parsed later:
word docID count docID count ....
word2...
...
*/
void index_save(index_t* index, FILE* fp){
	hashtable_iterate(index, fp, counters_saver);
}


/**
loads an index by parsing the file created by index_save
returns the loaded index
*/
index_t* index_load(FILE* fp, int size){
	char* curline; //can read a line up to 1000 chars long
	index_t* idx = index_new(size);
	while ((curline = freadlinep(fp)) != NULL){ //get a line
		char delim[] = " ";
		char* word = strtok(curline, delim); //first strtok takes in curline
		char* count;
		char* docID;
		while((docID = strtok(NULL,delim)) != NULL && (count = strtok(NULL,delim)) != NULL){ //insert each doc and set its count
			index_insert(idx, word, atoi(docID));
			counters_set(hashtable_find(idx, word), atoi(docID), atoi(count));
		}
		free(curline);
	}
	
	return idx;
}

//uncomment this line and run the following commands to unit test the index
//#define UNIT_TEST
//	mygcc -I../libcs50 index.c ../libcs50/libcs50.a -o index_unittest
//	./index_unittest
#ifdef UNIT_TEST
int main(int argc, char const *argv[])
{

	index_t* idx = index_new(10);
	index_insert(idx, "word1", 1);
	index_insert(idx, "word1", 1);
	index_insert(idx, "word1", 1);
	index_insert(idx, "word1", 2);
	index_insert(idx, "word2", 2);
	index_print(idx, stdout);

	printf("Testing save\n");
	FILE* fp = fopen("test","w");
	index_save(idx, fp);
	fclose(fp);
	
	printf("deleting index\n");
	index_delete(idx);

	printf("openingfile\n");
	fp = fopen("test","r");
	printf("testingload\n");
	idx = index_load(fp);
	fclose(fp);
	printf("printing index created from load\n");
	index_print(idx,stdout);
	index_delete(idx);
	return 0;
}
#endif
