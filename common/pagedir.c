#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "pagedir.h"
#include "webpage.h"


void savepage(int ID, webpage_t* page, char* dirname){
	char* filename = malloc(strlen(dirname)+10); //make the filename with space for /, end char, and u to 8 digits
	sprintf(filename, "%s/%d",dirname,ID);
	FILE* fp = fopen(filename, "w");
	if(fp == NULL){
		fprintf(stderr,"Page could not be saved\n");
		free(filename);
		return;
	}
	fprintf(fp, "%s\n", webpage_getURL(page));
	fprintf(fp, "%d\n", webpage_getDepth(page));
	fprintf(fp, "%s\n", webpage_getHTML(page));
	fclose(fp);
	free(filename);
}