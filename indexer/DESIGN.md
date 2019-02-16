#Design of the TSE Indexer

##Requirements
	The 'indexer' must create a reverse index of the words in all the webpage files in the pagedirectory
##Index Design
	The 'index' is a reverse index that maps from a word to all the files that have that word in them and a count of how many times the word appears
	The structure of the 'index' is a hashtable of counters.
	The keys for the hashtable are the words in the files,
	the counters store the ID of the file and the count of the word in the file
	The 'index' is designed to be saved to a file in the format:
	word docID count docID count ...
	word docID count docID count ...
	The index also has a load function that creates an index by parsing the saved file
##Indexer Design
	The 'indexer' checks the parameters passed in to make sure the first is a pageDirectory and the second is a valid filename for saving the index
	Then the 'indexer' calls build index to create the index from files made by the 'crawler'
###Build Index
	Build Index loops through all the files and loads them into a webpage struct pointer
	Then it calls indexPage to add all the words in the page into a file
###Index Page
	Index page loops through the words in a given webpage and adds each of them to the passed index

