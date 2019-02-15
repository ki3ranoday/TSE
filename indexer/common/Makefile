# Makefile for 'common' module
#
# David Kotz, April 2016, 2017
# updated by Xia Zhou, July 2016

# PROG =
OBJS = memory.o readlinep.o
LIBS =

# uncomment the following to turn on verbose memory logging
# TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING)
MAKE = make
CC = gcc

default: $(OBJS)

memory.o: memory.h

readlinep.o: readlinep.h

.PHONY: clean

clean:
	rm -f *~ *.o *.dSYM
