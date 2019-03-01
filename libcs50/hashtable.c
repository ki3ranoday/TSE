#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "set.h"
#include "jhash.h"

typedef struct hashtable_t{
	unsigned long size;
	struct set** table;
} hashtable_t;

/* Create a new (empty) hashtable; return NULL if error. */
hashtable_t* hashtable_new(const int num_slots){
	hashtable_t* ht = malloc(sizeof(hashtable_t));
	if(ht == NULL || num_slots <= 0)
		return NULL;

	ht->table = calloc(num_slots, sizeof(set_t*)); //initialize the table
	ht->size = (unsigned long) num_slots;
	for(int i = 0; i < num_slots; i ++)
		ht->table[i] = set_new();  //initialize all the sets
	return ht;
}

/* Insert item, identified by key (string), into the given hashtable.
 * The key string is copied for use by the hashtable.
 * Return false if key exists in ht, any parameter is NULL, or error;
 * return true if new item was inserted.
 */
bool hashtable_insert(hashtable_t *ht, const char *key, void *item){
	if(ht != NULL && key != NULL && item != NULL){
		//key will be copied by the set don't need to copy it for the table
		if(!set_insert(ht->table[JenkinsHash(key,ht->size)], key, item)){
			//if the insert failed the key is already in the table
			return false;
		}
		return true; //insert worked ok
	}
	return false; //one of the params was NULL
}

/* Return the item associated with the given key;
 * return NULL if hashtable is NULL, key is NULL, key is not found.
 */
void *hashtable_find(hashtable_t *ht, const char *key){
	if(ht != NULL && key != NULL){
		return set_find(ht->table[JenkinsHash(key,ht->size)], key); //returns the item or NULL if not found
	}
	return NULL;
}

/* Print the whole table; provide the output file and func to print each item.
 * Ignore if NULL fp. Print (null) if NULL ht.
 * Print a table with no items if NULL itemprint.
 */
void hashtable_print(hashtable_t *ht, FILE *fp,
                     void (*itemprint)(FILE *fp, const char *key, void *item)){
	fprintf(fp,"[\n");
	for(int i = 0; i < ht->size; i++){
		set_print(ht->table[i], fp, itemprint);
		fprintf(fp,"\n");
	}
	fprintf(fp,"]\n");
}

/* Iterate over all items in the table; in undefined order.
 * Call the given function on each item, with (arg, key, item).
 * If ht==NULL or itemfunc==NULL, do nothing.
 */
void hashtable_iterate(hashtable_t *ht, void *arg,
               void (*itemfunc)(void *arg, const char *key, void *item) ){
	for(int i = 0; i < ht->size; i++){
		set_iterate(ht->table[i], arg, itemfunc);
	}
}


/* Delete the whole hashtable; ignore NULL ht.
 * Provide a function that will delete each item (may be NULL).
 */
void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item) ){
	for(int i = 0; i < ht->size; i++){
		set_delete(ht->table[i], itemdelete);
	}
	free(ht->table);
	free(ht);
}