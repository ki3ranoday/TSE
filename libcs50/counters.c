#include <stdlib.h>
#include <stdio.h>
#include "counters.h"


//Define the countnode struct and give it a constructor
typedef struct countnode{
	int key;
	int counter;
	struct countnode* next;
} countnode_t;

static countnode_t* countnode_new(const int key){
	countnode_t* ctr = malloc(sizeof(countnode_t));
	if(ctr == NULL)
		return NULL;
	ctr->counter = 1;
	ctr->key = key;
	ctr->next = NULL;
	return ctr;
}

//define the counters struct and the methods that it has
typedef struct counters{
	struct countnode* head;
}counters_t;


/* Create a new (empty) counter structure; return NULL if error. */
counters_t *counters_new(void){
	counters_t* ctrs = malloc(sizeof(counters_t));
	if(ctrs == NULL)
		return NULL;
	ctrs->head = NULL;
	return ctrs;
}
/* Increment the counter indicated by key; key must be >= 0.
 * If the key does not yet exist, create a counter for it and initialize to 1.
 * Return 0 if ctrs is NULL or key is negative.
 * Return the new value of the counter related to the inserted key
 */
int counters_add(counters_t *ctrs, const int key){
	if(ctrs != NULL && key >= 0){
		countnode_t* current = ctrs->head;
		//try to find the key
		while(current != NULL){
			if(key == current->key){ //if you find the key 
				current->counter++; //increment its count
				return current->counter; //return the new count
			}
			current = current->next;
		}
		//otherwise add a new countnode at the head
		countnode_t* new = countnode_new(key);
		new->next = ctrs->head;
		ctrs->head = new;
		return 1;
	}
	return 0; //ctrs is null or key is negative
}

/* Return current value of counter associated with the given key;
 * return 0 if ctrs is NULL or if key is not found.
 */
int counters_get(counters_t *ctrs, const int key){
	countnode_t* current = ctrs->head;
	//try to find the key
	while(current != NULL){
		if(key == current->key){ //if you find the key 
			return current->counter; //return its count
		}
		current = current->next;
	}
	return 0;
}

/* Set the current value of counter associated with the given key;
 * If the key does not yet exist, create a counter for it and initialize to
 * the given value. Ignore if ctrs is NULL, if key < 0, or count < 0.
 */
void counters_set(counters_t *ctrs, const int key, int count){
	countnode_t* current = ctrs->head;
	//try to find the key
	while(current != NULL){
		if(key == current->key){ //if you find the key 
			current->counter = count; //set its count
			return;
		}
		current = current->next;
	}
	//didn't find the key insert the new node
	if(key >= 0){
		countnode_t* new = countnode_new(key);
		new->counter = count;
		new->next = ctrs->head;
		ctrs->head = new;
	}
}

/* Print all counters; provide the output file.
 * Ignore if NULL fp. Print (null) if NULL ctrs.
 */
void counters_print(counters_t *ctrs, FILE *fp){
	if(fp != NULL){
		if(ctrs == NULL){
			fprintf(fp,"(null)");
		}
		countnode_t* current = ctrs->head;
		fprintf(fp,"key\tcount\n");
		while(current != NULL){
			fprintf(fp,"%d\t%d\n",current->key, current->counter);
			current = current->next;
		}
	}
}

/* Iterate over all counters in the set (in undefined order):
 * call itemfunc for each item, with (arg, key, count).
 * If ctrs==NULL or itemfunc==NULL, do nothing.
 */
void counters_iterate(counters_t *ctrs, void *arg,
                      void (*itemfunc)(void *arg, const int key, int count)){
	if(ctrs != NULL && itemfunc != NULL){
		countnode_t* current = ctrs->head;
		while(current != NULL){
			(*itemfunc)(arg, current->key, current->counter); //call the function by dereferencing
			current = current->next;
		}
	}
}

/* Delete the whole counters. ignore NULL ctrs. */
void counters_delete(counters_t *ctrs){
	if(ctrs != NULL){
		countnode_t* current = ctrs->head;
		while(current != NULL){
			countnode_t* next = current->next;
			free(current);
			current = next;
		}
		free(ctrs);
	}
}