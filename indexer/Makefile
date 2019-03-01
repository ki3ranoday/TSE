# Makefile for indexer.c
#   builds executable indexer
#
# CS50 

# object files, and the target library
OBJS = indexer.o 
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common
CC = gcc
LIBS = ../common/common.a ../libcs50/libcs50.a

# Build the library by archiving object files
indexer: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $@

# Dependencies: object files depend on header files
indexer.o:

.PHONY: clean 

# clean up after our compilation
clean:
	rm -f pages/*
	rm -f indexer
	rm -f *.o

