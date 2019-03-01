#ifndef __COUNTERS_H
#define __COUNTERS_H
typedef struct counters counters_t;

/* Create a new (empty) counter structure; return NULL if error. */
counters_t *counters_new(void);

/* Increment the counter indicated by key; key must be >= 0.
 * If the key does not yet exist, create a counter for it and initialize to 1.
 * Ignore if ctrs is NULL or key is negative.
 * Return the new value of the counter related to the inserted key
 */
int counters_add(counters_t *ctrs, const int key);

/* Return current value of counter associated with the given key;
 * return 0 if ctrs is NULL or if key is not found.
 */
int counters_get(counters_t *ctrs, const int key);

/* Set the current value of counter associated with the given key;
 * If the key does not yet exist, create a counter for it and initialize to
 * the given value. Ignore if ctrs is NULL, if key < 0, or count < 0.
 */
void counters_set(counters_t *ctrs, const int key, int count);

/* Print all counters; provide the output file.
 * Ignore if NULL fp. Print (null) if NULL ctrs.
 */
void counters_print(counters_t *ctrs, FILE *fp);

/* Iterate over all counters in the set (in undefined order):
 * call itemfunc for each item, with (arg, key, count).
 * If ctrs==NULL or itemfunc==NULL, do nothing.
 */
void counters_iterate(counters_t *ctrs, void *arg,
                      void (*itemfunc)(void *arg, const int key, int count));

/* Delete the whole counters. ignore NULL ctrs. */
void counters_delete(counters_t *ctrs);
#endif