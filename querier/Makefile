# Makefile for querier.c
#   builds executable indexer
#
# CS50 

# object files, and the target library
OBJS = querier.o
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common
CC = gcc
LIBS = ../common/common.a ../libcs50/libcs50.a

# Build the library by archiving object files
querier: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $@

# Dependencies: object files depend on header files
querier.o:

.PHONY: clean 

# clean up after our compilation
clean:
	rm querier
	rm *.o

