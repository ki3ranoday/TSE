#Crawler Testing
My testing file runs the crawler with three seed URLs:
	http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/
	http://old-www.cs.dartmouth.edu/~cs50/index.html			
	http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/
The test runs 'make clean' and 'make' before each test
After the urls are tested on different depths, the
test checks how many files are in pages and if that is the 
correct number of files according to the info Prof Xia sent us.

if all the urls print out "good" the test has passed