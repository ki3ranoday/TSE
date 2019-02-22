#include <stdbool.h>
#include <stdio.h>
#include "hashtable.h"

typedef hashtable_t index_t;

/**
create a new empty hashtable, return null if error
*/
index_t* index_new(int num_slots);

/*
insert a word from a document into the index
the word is the key for the hashtable,
insert new counter if word not yet in table
the counter has all the docIds the word is in and their counts
return false if failed to insert
*/
bool index_insert(index_t* index, char* word, int docID);

/*
return the count of the word in the document
return less than 1 if any error(count of a word can't be zero because when you add it the first time it is 1)
*/
int index_find(index_t* index, char* word, int docID);

/*
print out the hashtable by printing each counter use the print functions for each
*/
void index_print(index_t* index);

/* 
Delete the whole index; ignore NULL ht.
*/
void index_delete(index_t* index);

/*
prints the index to a file so it can be parsed later:
word docID count docID count ....
word2...
...
*/
void index_save(index_t* index, FILE* fp);

/**
loads an index by parsing the file created by index_save
returns the loaded index
*/
index_t* index_load(FILE* fp, int size);
