#Kieran O'Day README for the Querier

The Querier is the final piece of out Tiny Search Engine
The Querier takes queries from the user, and searches our 
data base of webpages for the words that the user put in.

To run the querier, make it using 'make' then run using:

./querier ../crawler/pages ../indexer/indexfile

you may need to make the pages and index file before
testing. to do so, use the crawler with
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ pages
and then use the indexer with
./indexer ../crawler/pages indexfile

```c
/*
	runs the querier to get input from the user and print it on the screen
*/
void run(const char* pagedir, index_t* idx, bag_t* cleanupbag)
/*
	checks that there are the correct numbers of arguments
	checks that the page directory exists
	checks that the index file exists and is readable
*/
int checkArgs(int argc, char const *argv[])
/*
	checks if the query string is good:
	only alphabetical and spaces in the string
	no operators at beginning or end of string
	no two operators in a row
*/
bool goodQuery(char* queryString)
/*
	queries the index for the words
	takes in a queryString that is already checked
	uses the mergers to process the query string and create a final set of documents
*/
counters_t* query(index_t* idx, char* queryString, bag_t* cleanupbag)
/*
	queries the index for one word
	really just findes the word in the hashtable and returns that word's counter
*/
counters_t* queryWord(index_t* idx, char* word, bag_t* cleanupbag)
/*
	very simple function to find the min of two ints
	returns the minimum of the two
*/
int min(int one, int two)
/*
	merges one and two based on intersection for the AND operator
	counters one will be the new merged set
	pages in both counters will have their count set to the min of the two
	pages not in both counters will have their count set to zero
*/
void andMerge(counters_t* one, counters_t* two)
/*
	arg holds the counter you are iterating over as one and the counter you are intersecting as two
	this does the itersection for one key
	if both counters have the key
	the first counter sets its count to the min of the counts
	if they dont both have the key
	the first counter sets its count to 0
*/
void counters_intersector(void *arg, const int key, int count)
/*
	merges one and two based on union for the OR operator
	counters one will be the new merged set
	pages in both counters will have their count set to the sum of the two counters
	pages not in counters one will be added and their count will be set to their count in two
*/
void orMerge(counters_t* one, counters_t* two)
/*
	arg holds the two counters, you are iterating over two but setting changing one
	if one contains the same key as two, set one's count as the counts' sum
	if one does not contain the key, add the key to one and set its count to the count in two
*/
void counters_unioner(void *arg, const int key, int count)
/*
	parses the query string into an array of words of size numwords, and returns it
	returns NULL if the query string has non alpha, non space characters
	returns NULL on any error
*/
char** parseWords(char* queryString, int numWords)
/*
	makes sure that every character in the string is either alpha or a space
	stops at the terminating char
*/
bool onlyAlpha(char* queryString)
/*
	counts how many words are in a string
*/
int numWordsInQuery(char* queryString)
/*
counts the number of documents in a counter with score > 0
*/
int counters_size(counters_t* c)
/*
copys a counter and returns it
*/
counters_t* counters_copy(counters_t* c)
/*
	sorts the array of counters with score > 0
*/
void sort_helper(void* arg, const int key, int count)
/*
	sorts the counters with size > 0
*/
docscore_t** sortdocs(counters_t* c, int size)
```

To see more information on implementation, design and testing
check out the IMPLEMENTATION.md, DESIGN.md, and TESTING.md files