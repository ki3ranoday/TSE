#Implementation of TSE's 'Indexer'

##Indexer.c
### Check Parameters
	only three arguments
	opendir(arg[1]) != NULL directory exists
	fopen(arg[2]) != NULL file exists
	close the file and the directory
### Build the Index
	initialize an index
	for each file in pageDirectory
		load in a webpage, check if null
		add the page's words to the index
		for each word
			make it lower case
			index_insert the word
			free the word
### Save the Index
### Clean up
	index
	file pointer


##Index.c
### Typedef
	'index' is a special type of hashtable where the key is a word and the value is a counters set
	typedef hashtable_t index_t;
### Insert
	check if the word is already in the hashtable
	if no
		initialize a counter struct and add the word and new counter to the hashtable
	add the docID to the counter at the counter at hashtable_get(word)
### Find
	check if the word is in the hashtable
	return the count of the docID in the counters at the word's value
### Print
	use a helper method that just calls the counters print method in hashtable print
### Delete
	hashtable delete the index with counters delete passed in through a helper method
### Save
	needs two helper methods: one to be called by hashtable iterate and one to be called by counters iterate
	the first helper prints the word onto the line and then calls the second helper with hashtable iterate
	the second helper prints each docID count pair onto the same line
	the first helper then prints a \n char
	this happens for every key in hashtable
### Load
	initialize an index
	read in each line
		add the word and an empty counter to the hashtable
		add each docID to the hashtable and set each count
	return the index

