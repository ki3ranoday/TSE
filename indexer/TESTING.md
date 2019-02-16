#Indexer Testing
My testing file runs the crawler to create page directories and then runs the indexer to index them.
In my test I use the first and third URLS at depths 5 and 1 respectively
	http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/
	http://old-www.cs.dartmouth.edu/~cs50/index.html			
	http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/
The test runs 'make clean' and 'make' for the crawler before each test
The test makes the indexer at the beginning of the run
The test saves the results to two files and echos the contents of the files to stdout
The user can then check the results by looking to see that they look correct

#Index Unit Testing
The 'index' unit test creates a very simple 'index' by manually adding two words to the 'index' with different 
documents and number of times adding
This 'index' is printed in stdout and saved to a file
The 'index' is deleted
Then the 'index' is loaded from the file
and printed again to the stdout so the user can check that the load did indeed create the same 'index' as 
before
