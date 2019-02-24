#Implementation for TSE 'Querier.c' 
####Kieran O'Day

## main
check the argumentss
make the index
make a cleanup bag
run the query
clean up


###Querier
get user input
	break input into array of words and make all the words lowercase
	check input
		can't start or end with an 'and' or 'or' operator
		no non-alphabetic characters
	query the input
	sort the output of the query
	print the sorted output with urls from the document files
	clean up
	repeat

###Query
keep a sum counter and a product counter
go through all the query words
if you see an and ignore it
if you see an or union the sum and product and reset the product to NULL
if you see a word find the intersect between the product
 	and the word, if the product is not null, 
 	otherwise just set the product to that word's query 
 	from the index
union the product and sum at the end

### union helper
set the document and count of the first counter to that passed 
key and count from the second counter

### union
counter iterate over the second counter and call the helper

### intersection
iterate over the first counter and call counters iterate
the arg must be a counter pair struct
find the similarities between counters
only keep a counter if both counters have the document
	if they do, make the count for that doc the min of
	the its counts from the two counters
if a doc is not in both counters set its count to zero 
to ignore later

### sort
count the number of documents in a counter
for each one, use a bubble sort after inserting it into 
the array to build the sorted array

##Implementation
see the 'IMPLEMENTATION.md' file