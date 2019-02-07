/**
* Crawler.c 
* 
* Kieran O'Day
* February 2019
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "bag.h"
#include "hashtable.h"
#include "pagedir.h"
#include "webpage.h"

#define HASHTABLESIZE 100

/* helper function initialization; see function definition for comments */
void crawl(webpage_t* seed, int maxDepth, char* dirname);
int checkInput(int argc, char* argv[], webpage_t** seed);
webpage_t* makeWebpage(char* URL, int depth, hashtable_t* ht);
void dontDelete(void* item){} //does nothing because for this hashtable, the item is just code memory so we dont need to delete it

/*
* 'main' for crawler.c
* checks the input then runs crawl
*
* error codes (from checkInput)
*	0: input is good
*	1: wrong number of args
*	2: bad seedURL (not internal, normilizable, failed to make, or failed to fetch)
*	2: bad pageDirectory
*	3: bad depth
*/
int main(int argc, char* argv[]){
	webpage_t** seedptr = malloc(sizeof(webpage_t*)); //make a ptr to the seed webpage
	//check input
	//check all the errors, if no error seed will be initialized with argv[1];
	int error = checkInput(argc, argv, seedptr);
	if(error != 0){ //if any error 
		if(seedptr != NULL) 
			free(seedptr);//clean up seedptr, *seedptr would be freed if necessary by checkinput
		//return the error code
		return error;
	}
	//does the crawl algorithm
	int maxd = atoi(argv[3]); //there wont be an error here because checkInput did this fine
	crawl(*seedptr, maxd, argv[2]); 
	return 0; // no errors
}

/**
* crawl starts with a seed webpage,a maxdepth int, and a directory name.
* from that seed, it archives the webpage into the directory, then 
* goes to all the urls on the html page (aka the page's neighbors),
* if those have not been archived yet, the crawler saves the htmls
* then continues until it reaches its max depth
*/
void crawl(webpage_t* seed, int maxDepth, char* dirname){

	bag_t* toVisit = bag_new(); //holds list of URLs to visit
	hashtable_t* visited = hashtable_new(HASHTABLESIZE); //hashtable of visited URLs
	bag_insert(toVisit, seed); //insert the seeds URL to visited and toVisit
	hashtable_insert(visited, webpage_getURL(seed), "-");
	webpage_t* current;
	int nextID = 1;
	while ((current = bag_extract(toVisit)) != NULL){
		//rather than fetching the page here, I will fetch each page before I add them to toVisit
		savepage(nextID, current, dirname); //save page 
		nextID++; //increment nextID
		if (webpage_getDepth(current) < maxDepth){ //check depth
			char** url = malloc(sizeof(char**));
			int pos = 0;
			while ((pos = webpage_getNextURL(current, pos, url)) >= 0){ //go through the pages urls
				webpage_t* newpage = makeWebpage(*url, webpage_getDepth(current)+1, visited);
				if(newpage != NULL){
					//if the makeWebpage was successful, the url was valid and inserted into visited,
					//and newpage is a webpage with its html already fetched
					bag_insert(toVisit, newpage);  
				}//else dont add newpage to the bag
				free(*url); //clean up url
			}
			free(url);
		}
		webpage_delete(current); //clean up that webpage
	}
	//clean up hashtable and bag
	hashtable_delete(visited, dontDelete);
	bag_delete(toVisit, webpage_delete);
}

/*
* checkInput checks the input provided by the user,
* if the input is good, it also initializes a webpage seed with the seedURL
* error codes: 
*	0: input is good
*	1: wrong number of args
*	2: bad seedURL
*	2: bad pageDirectory
*	3: bad depth
*/
int checkInput(int argc, char* argv[], webpage_t** seed){
	if(argc != 4){ 
		fprintf(stderr, "Usage: ./crawler seedURL pageDirectory maxDepth\n");
		return 1;
	}
	//tries to make the seed webpage
	if((*seed = makeWebpage(argv[1], 0, NULL)) == NULL){
		return 2;
	}
	//makes a filename of a .crawler file in the pageDirectory
	char* filename = malloc(strlen(argv[2])+strlen("/.crawler") + 1);
	sprintf(filename, "%s/.crawler", argv[2]);
	FILE* fp = fopen(filename, "w");
	free(filename); //clean up filename
	//make sure the directory name is good
	if(fp == NULL){
		fprintf(stderr, "Page directory %s does not exist or is not writable\n", argv[2]);
		return 2;
	}
	fclose(fp); 
	//make sure argv3 is an int and is greater than 0 (if not an int atoi would convert to 0)
	int maxd = atoi(argv[3]);
	if(maxd < 1){ 
		fprintf(stderr, "maxDepth must be an integer greater than 0\n");
		return 3;
	}
	return 0;
}

/**
* makeWebpage tries to make a webpage;
* returns NULL on any error and prints the error
* checks if the URL is:
*	non-null,
*	internal, 
*	normilizable,
*	makeable,
*	fetchable.
* if the hashtable is null, the function will only do this
*
* if the hashtable parameter is not null:
* before making the webpage the function will try to insert the url into the ht
*	returns NULL if the URL is already in the ht
*note*
* 	//the ht param is a little strange, but makes this code reusable for both
*   the seed in main, and then every other url because the only difference is the 
*	adding of the webpage to the hashtable.
*/
webpage_t* makeWebpage(char* URL, int depth, hashtable_t* ht){
	if(URL == NULL){ //check null
		fprintf(stderr, "Null URL\n");
		return NULL;
	}
	//make sure URL is good
	if(!NormalizeURL(URL)){ //check normilizable
		fprintf(stderr, "%s is not normalizable\n", URL);
		return NULL;
	}
	if(!IsInternalURL(URL)){ //check internal
		fprintf(stderr, "%s is not internal\n", URL);
		return NULL;
	}
	if(ht != NULL){ //if the ht parameter is a hashtable
		if(!hashtable_insert(ht, URL, "-")){ //try to insert the URL, only continue if URL is new
			//no error code this happens really often and isnt really an error
			return NULL; //if the URL is already in the table, return null
		}
	}
	webpage_t* page = webpage_new(URL, depth, NULL);
	if(page == NULL){ //try to make the seed page
		fprintf(stderr, "Failed to make webpage %s\n", URL);
		return NULL;
	}
	if(!webpage_fetch(page)){ //try to fetch the seed page html
		fprintf(stderr, "Failed to fetch html from %s\n", URL);
		webpage_delete(page);
		return NULL;
	}
	return page;
}