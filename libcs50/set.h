/* 
 * set.h - header file for 'set' module
 * 
 * A 'set' is an unordered collection of items accessible by the items' key.
 * The set starts empty and items can be added and removed from the set
 * The set can contain any number of items
 *
 * See README for more information
 *
 * Kieran O'Day, January 2019
 * 	The below code is copied from the lab3 page for cs50, functions are implemented in set.c
 */
#include <stdbool.h>

#ifndef __SET_H
#define __SET_H

typedef struct set set_t;

/* Create a new (empty) set; return NULL if error. */
set_t *set_new(void);

/* Insert item, identified by a key (string), into the given set.
 * The key string is copied for use by the set.
 * Return false if key exists, any parameter is NULL, or error;
 * return true iff new item was inserted.
 */
bool set_insert(set_t *set, const char *key, void *item);

/* Return the item associated with the given key;
 * return NULL if set is NULL, key is NULL, or key is not found.
 */
void *set_find(set_t *set, const char *key);

/* Print the whole set; provide the output file and func to print each item.
 * Ignore if NULL fp. Print (null) if NULL set.
 * Print a set with no items if NULL itemprint.
*/
void set_print(set_t *set, FILE *fp,
               void (*itemprint)(FILE *fp, const char *key, void *item) );

/* Iterate over all items in the set, in undefined order.
 * Call the given function on each item, with (arg, key, item).
 * If set==NULL or itemfunc==NULL, do nothing.
 */
void set_iterate(set_t *set, void *arg,
                 void (*itemfunc)(void *arg, const char *key, void *item) );

/* Delete the whole set; ignore NULL set.
 * Provide a function that will delete each item (may be NULL).
 */
void set_delete(set_t *set, void (*itemdelete)(void *item) );
#endif