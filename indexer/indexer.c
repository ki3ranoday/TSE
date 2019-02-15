#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include "index.h"
#include "pagedir.h"
#include "word.h"

#define IDXSIZE 100 //make a big index

int checkparams(int argc, char *argv[]);
index_t* build_index(char* pageDirectory,char* indexFilename);
void indexPage(index_t* idx, webpage_t* page, int ID);


int main(int argc, char *argv[])
{
	//check parameters
	int error = checkparams(argc, argv);
	if(error != 0)
		return error;
	//index the pages
	index_t* idx = build_index(argv[1], argv[2]);
	FILE* fp = fopen(argv[2], "w");
	index_save(idx, fp);
	index_delete(idx);
	fclose(fp);
	return 0;
}

/**
checks the parameters
return codes:
	1:wrong number of parameters
	2:bad pagedirectory name
	3:bad indexfile name
	0:no error
*/
int checkparams(int argc, char *argv[]){
	if(argc != 3){
		fprintf(stderr, "Usage: ./indexer pageDirectory indexFilename\n");
		return 1;
	}
	DIR* dirtest = opendir(argv[1]);
	if(dirtest != NULL){
		closedir(dirtest);
	} else {
		fprintf(stderr, "Failed to open page directory %s\n", argv[1]);
		return 2;
	}
	FILE* filetest = fopen(argv[2], "w");
	if(filetest != NULL){
		fclose(filetest);
	}else{
		fprintf(stderr, "Failed to open index file %s\n", argv[2]);
		return 3;
	}
	return 0;
}

/**
builds an index from a directory of webpages by looping 
through and using index page to index each page
returns the built index
*/
index_t* build_index(char* pageDirectory, char* indexFilename){
	int curID = 1;
	index_t* idx = index_new(IDXSIZE);
	webpage_t* page = NULL;
	while((page = loadpage(curID,pageDirectory)) != NULL){
		indexPage(idx, page, curID);
		curID ++;
		webpage_delete(page);
	}
	return idx;
}

/*
adds all of the words on a page to the given index.
index_insert does the brunt of the work
*/
void indexPage(index_t* idx, webpage_t* page, int ID){
	char* word;
	int pos = 0;
	while((pos = webpage_getNextWord(page, pos, &word)) > 0){
		makelower(word);
		index_insert(idx, word, ID);
		free(word);
	}
}










