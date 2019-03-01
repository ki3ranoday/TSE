/*
*  set.c - 'set' module
*  !!!based on the cs50 'bag' module cs50 code!!!
*  
*  see set.h for more information
*
*  Kieran O'Day
*  January 2019
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "set.h"

typedef struct setnode{
	const char* key; //pointer to the key for this data
	void* item; //pointer to the data
	struct setnode* next; //pointer to the next node
} setnode_t;

typedef struct set{
	struct setnode* head;
} set_t;

/*** local functions ***/
static setnode_t* setnode_new(const char* key, void* item);

/* Create a new (empty) set; return NULL if error. */
set_t* set_new(void){
	set_t* set = malloc(sizeof(set_t));
	if(set == NULL){
		return NULL; //error allocating memory
	}else{ //initialize contents of the set
		set->head = NULL;
		return set;
	}
}

/* Insert item, identified by a key (string), into the given set.
 * The key string is copied for use by the set.
 * Return false if key exists, any parameter is NULL, or error;
 * return true if new item was inserted.
 */ //copied from bag slight modification
bool set_insert(set_t *set, const char *key, void *item){
	if(set != NULL && key != NULL && item != NULL){ //make sure params arent null
		if(set_find(set, key) == NULL){ //if the key doesn't already exist
			char* newkey = malloc(sizeof(char) * (strlen(key) +1));
			strcpy(newkey,key); //copy the key
			setnode_t* new = setnode_new(newkey, item); //make the new node
			new->next = set->head; //insert the new node into the linked list at the head
			set->head = new;
			return true;
		}
	}
	return false;
}
 
/* Return the item associated with the given key;
 * return NULL if set is NULL, key is NULL, or key is not found.
 */
void* set_find(set_t *set, const char *key){
	if(set != NULL && key != NULL){
		setnode_t* current = set->head;
		while(current != NULL){ //loop through the list
			if(strcmp(current->key,key) == 0){
				return current->item; //return the item if the key matches
			}
			current = current->next; //move to the next node
		}
	}
	return NULL;
}

/* Print the whole set; provide the output file and func to print each item.
 * Ignore if NULL fp. Print (null) if NULL set.
 * Print a set with no items if NULL itemprint.
*/ //copied from bag slight modification
void set_print(set_t *set, FILE *fp,
               void (*itemprint)(FILE *fp, const char *key, void *item) ){
	if(fp != NULL){
		if(set != NULL){
			fputc('{',fp);
			for (setnode_t* current = set->head; current != NULL; current = current->next) {
        if (itemprint != NULL) {  // print the node's item 
          fputc(' ', fp);
          (*itemprint)(fp, current->key, current->item); 
        }
      }
			fputs(" }",fp);
		}else{
			printf("null");
		}
	}
}

/* Iterate over all items in the set, in undefined order.
 * Call the given function on each item, with (arg, key, item).
 * If set==NULL or itemfunc==NULL, do nothing.
 */
void set_iterate(set_t *set, void *arg,
                 void (*itemfunc)(void *arg, const char *key, void *item) ){
	if (set != NULL && itemfunc != NULL) {
    // call itemfunc with arg, on each item
    for (setnode_t* current = set->head; current != NULL; current = current->next) {
      (*itemfunc)(arg, current-> key, current->item); 
    }
  }
}

/* Delete the whole set; ignore NULL set.
 * Provide a function that will delete each item (may be NULL).
 */ //copied from bag slight modifications (for -> while loop)
void set_delete(set_t *set, void (*itemdelete)(void *item) ){
	if(set != NULL){
		setnode_t* current = set->head;
		while(current != NULL){
			if(itemdelete != NULL) { //if you can 
				(*itemdelete)(current->item); //delete the current item
			}
			setnode_t* next = current->next; //remember next
			free((void*)(current->key)); //free the key
			free(current); //free current item
			current = next; //move to next
		}
		free(set);//free the set
	}
}

//make a new setnode modified from cs50 bag.c
static setnode_t* setnode_new(const char* key, void* item){
	setnode_t* node = malloc(sizeof(setnode_t));
  	if (node == NULL) {
	  // error allocating memory for node; return error
	  return NULL;
	} else {
	  node->item = item;
	  node->key = key;
	  node->next = NULL;
	  return node;
  	}
}





