#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "pagedir.h"
#include "webpage.h"
#include "readlinep.h"

/**
Used by crawler to save a page into the page directory,
first line is the page URL
second line is the page depth
everything else is the HTML
*/
void savepage(int ID, webpage_t* page, char* dirname){
	char filename[strlen(dirname)+8]; //make the filename with space for /, end char, and up to 6 digits
	sprintf(filename, "%s/%d",dirname,ID);
	FILE* fp = fopen(filename, "w");
	if(fp == NULL){
		fprintf(stderr,"Page could not be saved\n");
		return;
	}
	fprintf(fp, "%s\n", webpage_getURL(page));
	fprintf(fp, "%d\n", webpage_getDepth(page));
	fprintf(fp, "%s\n", webpage_getHTML(page));
	fclose(fp);
}
webpage_t* loadpage(int ID, char* dirname){
	char filename[strlen(dirname) + 8]; //enough memory for pageDirectory a '/' a number up to 6 digits and a '\0'
	sprintf(filename,"%s/%d",dirname,ID); //make the filename
	FILE* fp = fopen(filename, "r"); //open the file to read
	if(fp == NULL){
		return NULL;
	}
	char* url = freadlinep(fp);
	char* depthstr = freadlinep(fp);
	int depth = atoi(depthstr);
	free(depthstr);
	char* html = freadlinep(fp); //not going to be the whole html so i'll keep adding on lines below:
	char* newline;
	while ((newline = freadlinep(fp)) != NULL){
		char* biggerHtml = malloc(strlen(html) + strlen(newline) + 1);
		sprintf(biggerHtml,"%s%s",html,newline);
		free(html);
		free(newline);
		html = biggerHtml;
	}
	fclose(fp);
	webpage_t* result = webpage_new(url, depth, html);
	free(url);
	return result;
}