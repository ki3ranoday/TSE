#!/bin/bash
#
#   test.sh
#
#   tests the crawler algortithm with:
#		seedURL														depths  	expected number of files
#	http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/		1,2,3,4,5	2,4,7,9,10
#	http://old-www.cs.dartmouth.edu/~cs50/index.html				1,2			3,3
#	http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/		1,2			7,1705
#		The third url I ran once at depth 2 and it produced 1705 files, but took about 30 min,
#		(a one second delay for each of 1705 files alone means a total wait time of 28.416 minutes);
#
#    Kieran O'Day
#    Jan 09, 2019

URL1="http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/"
URL2="http://old-www.cs.dartmouth.edu/~cs50/index.html"
URL3="http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/"


NUMS=(2 4 7 9 10)
for i in 1 2 3 4 5
do
	make clean
	make
	./crawler $URL1 pages $i
	if [ $(ls pages | wc -w) -eq ${NUMS[$i-1]} ]; then
		echo "$URL1 good at depth $i"
	else
		echo $(ls pages | wc -w)
	fi
done

for i in 1 2
do 
	make clean
	make
	./crawler $URL2 pages $i
	if [ $(ls pages | wc -w) -eq 3 ]; then
		echo "$URL2 good at depth $i"
	else
		echo "$URL2 failed at depth $i"
	fi
done

make clean
make
./crawler $URL3 pages 1
if [ $(ls pages | wc -w) -eq 7 ]; then
	echo "$URL3 good at depth 1"
else
	echo "$URL1 failed at depth 1"
fi