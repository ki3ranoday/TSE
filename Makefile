# Makefile for CS50 Tiny Search Engine
#
# CS50

MAKE = make
.PHONY: all valgrind clean

############## default: make all libs except the libcs50 library because that one was made and provided by the instructor and I don't want to use my own lab3 implementations for it and programs ##########
all: 
	$(MAKE) -C libcs50
	$(MAKE) -C common
	$(MAKE) -C crawler
	$(MAKE) -C indexer
	$(MAKE) -C querier

############## valgrind all programs ##########
valgrind: all
	$(MAKE) -C crawler valgrind

############## clean  ##########
clean:
	rm -f *~
	$(MAKE) -C libcs50 clean
	$(MAKE) -C common clean
	$(MAKE) -C crawler clean
	$(MAKE) -C indexer clean
	$(MAKE) -C querier clean
