#!/bin/bash
#
#   test.sh
#
#   tests the indexer algortithm with page directories
# 	created by the first and third seedURLs
# 	saves the index in a file and prints the the file to the screen
#
#    Kieran O'Day
#    Jan 09, 2019

URL1="http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/"
URL2="http://old-www.cs.dartmouth.edu/~cs50/index.html"
URL3="http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/"

make clean
make #makes the indexer executable

#tests the indexer with the first URL
cd ../crawler
make clean
make
./crawler $URL1 pages 5
cd ../indexer
./indexer ../crawler/pages indexfile1
echo "index for first URL is :"
cat indexfile1

#tests the indexer with the third url directory
cd ../crawler
make clean
make
./crawler $URL3 pages 1
cd ../indexer
./indexer ../crawler/pages indexfile2
echo "index for third URL is :"
cat indexfile2

#test erroroneous input

echo "these should fail"
./indexer
./indexer notadirectory
./indexer notadirectory indexfile
./indexer ../crawler/pages ./notadirectory/file

