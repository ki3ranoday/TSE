# Makefile for pagedir library
#   Builds and compiles pagedirectory functions into a library
#
# CS50 

# object files, and the target library
OBJS = pagedir.o index.o readlinep.o word.o
LIB = common.a
LIBS = ../libcs50/libcs50.a
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50/
CC = gcc
MAKE = make

# Build the library by archiving object files
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

# Dependencies: object files depend on header files
pagedir.o: pagedir.h readlinep.h
readlinep.o: readlinep.h
index.o: index.h
word.o: word.h

.PHONY: clean 

# clean up after our compilation
clean:
	rm $(LIB)
	rm *.o
