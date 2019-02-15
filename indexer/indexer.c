#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include "index.h"
#include "pagedir.h"

#define IDXSIZE 100

int checkparams(int argc, char const *argv[]);
index_t* indexDirectory(char* pageDirectory, char* indexFilename);
void indexPage(index_t* idx, webpage_t* page, int ID);


int main(int argc, char const *argv[])
{
	//check parameters
	int error = checkparams(argc, argv);
	if(error != 0)
		return error;
	//index the pages
	index_t* idx = indexDirectory(argv[1], argv[2]);
	index_save(idx, stdout);
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
int checkparams(int argc, char const *argv[]){
	if(argc != 3){
		fprintf(stderr, "Usage: ./indexer pageDirectory indexFilename\n");
		return 1;
	}
	DIR* dirtest = opendir(argv[1]);
	if(dir != NULL){
		closedir(dirtest);
	} else {
		fprintf(stderr, "Failed to open page directory %s\n", argv[1]);
		return 2;
	}
	FILE* filetest = fopen(argv[2], "r");
	if(filetest != NULL){
		fclose(filetest);
	}else{
		fprintf(stderr, "Failed to open index file %s\n", argv[2]);
		return 3;
	}
	return 0;
}

index_t* indexDirectory(char* pageDirectory, char* indexFilename){
	int curID = 1;
	index_t idx = index_new(IDXSIZE);
	while((webpage_t* page = loadPage(curID,pageDirectory)) != NULL){
		indexPage(idx, page, curID);
		curID ++;
		webpage_delete(page);
	}
	return idx;
}

void indexPage(index_t* idx, webpage_t* page, int ID){
	char* word;
	int pos = 0;
	while((pos = webpage_getNextWord(page, pos, &word)) > 0){
		index_insert(idx, word, ID);
	}
}










