# Makefile for crawler.c
#   builds executable crawler
#
# CS50 

# object files, and the target library
OBJS = crawler.o
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common
CC = gcc
LIBS = ../common/common.a ../libcs50/libcs50.a

# Build the library by archiving object files
crawler: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $@

# Dependencies: object files depend on header files
crawler.o:

.PHONY: clean 

# clean up after our compilation
clean:
	rm -f pages/*
	rm crawler
	rm *.o

