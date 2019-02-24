#testing for the querier
####Kieran O'Day

##What do I test
To test my program, I wrote a file that contains a number
of test cases. The case build on one another so the user
can visually confirm their accuracy
For example: I test "computer", then "science", then 
"computer or science", then "computer and science". This
allows the user to look at the output for computer and 
science and make sure the union and intersection of the 
two works as it should.
The test also tests to show that whitespace does not 
matter, but not alphabetic characters trigger an error

The test concludes with several error cases and testing 
words that do not appear in any document

##How to Run the test
To run the test, simply run 
./test.sh
which will do this:
cat testcommands | querier ../crawler/pages ../indexer/indexfile

###note:
you may need to make the pages and index file before
testing. to do so, use the crawler with
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ pages
and then use the indexer with
./indexer ../crawler/pages indexfile