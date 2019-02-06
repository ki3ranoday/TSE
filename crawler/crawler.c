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

void crawl(webpage_t* seed, int maxDepth, char* dirname);
void dontDelete(void* item){} //does nothing because for this hashtable, the item is just code memory so we dont need to delete it
/*
* 'main' for crawler.c
* checks the input then runs crawl
*
* error codes: 
*	1: wrong number of args
*	2: bad seedURL (not internal, normalizable, or error on webpage_fetch or webpage_new)
*	3: bad pageDirectory
*	4: bad depth
*/
int main(int argc, char* argv[]){
	//check input
	//make sure there are the right number of arguments
	if(argc != 4){ 
		fprintf(stderr, "Usage: ./crawler seedURL pageDirectory maxDepth\n");
		return 1;
	}
	//make sure URL is good
	if(!IsInternalURL(argv[1])){ //check internal
		fprintf(stderr, "Seed %s is not internal\n", argv[1]);
		return 2;
	}
	if(!NormalizeURL(argv[1])){ //check normilizable
		fprintf(stderr, "Seed %s is not normalizable\n", argv[1]);
		return 2;
	}
	webpage_t* seed = webpage_new(argv[1], 0, NULL);
	if(seed == NULL){ //try to make the seed page
		fprintf(stderr, "Failed to make seed webpage %s\n", argv[1]);
		return 2;
	}
	if(!webpage_fetch(seed)){ //try to fetch the seed page html
		fprintf(stderr, "Failed to fetch seed html from %s\n", argv[1]);
		webpage_delete(seed);
		return 2;
	}
	//makes a filename of a .crawler file in the pageDirectory
	char* filename = malloc(strlen(argv[2])+strlen("/.crawler") + 1);
	sprintf(filename, "%s/.crawler", argv[2]);
	FILE* fp = fopen(filename, "w");
	free(filename);

	//make sure the directory name is good
	if(fp == NULL){
		fprintf(stderr, "Page directory %s does not exist or is not writable\n", argv[2]);
		return 3;
	}
	fclose(fp);
	//make sure argv3 is an int and is greater than 0 (if not an int atoi would convert to 0)
	int maxd = atoi(argv[3]);
	if(maxd < 1){ 
		fprintf(stderr, "maxDepth must be an integer greater than 0\n");
		return 4;
	}
	//does the crawl algorithm
	crawl(seed, maxd, argv[2]); 
	return 0; // no errors
}

/**
* crawl starts with a seed webpage,a maxdepth int, and a directory name.
* from that seed, it archives the webpage into the directory, and then 
* goes to all the urls on the html page, aka the page's neighbors,
* if those have not been archived yet, the crawler saves the htmls
* and then continues until it reaches its max depth
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
				if(*url == NULL){//normalize can fail because of a null url 
					fprintf(stderr, "null URL\n");
					continue; //skip null url; probably end of file
				}
				if(!NormalizeURL(*url)){
					fprintf(stderr, "could not normalize %s\n",*url);
					free(*url);
					continue; //skip this url if it couldnt be normalized
				}
				if(!IsInternalURL(*url)){
					//fprintf(stderr, "%s is not an internal URL\n",*url);
					free(*url);
					continue; //skip this url if its not internal
				}
				if(hashtable_insert(visited,*url, "-")){
					webpage_t* newpage = webpage_new(*url, webpage_getDepth(current)+1, NULL);
					if(newpage != NULL && webpage_fetch(newpage)){ //try to fetch the html
						bag_insert(toVisit, newpage);
					}else{
						fprintf(stderr, "Could not fetch HTML for %s\n",*url);
						webpage_delete(newpage);
					}
				} //else (failed insert) do nothing
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