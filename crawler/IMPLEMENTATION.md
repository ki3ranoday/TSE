#Crawler Implementation

Kieran O'Day - February 2019

## 'Crawler.c'
### 'main'
	initialize memory for a seed pointer
	check the input (initializes the seed pointed to by the seed pointer)
	return error code if applies
	run crawl
### 'crawl'
	make a bag of webpages toVisit
	make a hashtable of URLs visited, values are "-" (dont matter)
	add the passed in seed to the bag of webpages to crawl
	add the seed's URL to the hashtable of URLs seen
	intitialize next ID to 1
	while there are more webpages to crawl,
		save the page with savepage from pagedir.c with nextID
		increment nextID
		extract a webpage (URL,depth) item from the bag of webpages to be crawled,
		if the webpage depth is < maxDepth, explore the webpage to find links:
			use pagescanner to parse the webpage to extract all its embedded URLs;
			for each extracted URL,
			try to make page (will add URL to visited if successful)
			if successful, add the page to toVisit
		free url
	free the url pointer
	free visited and toVisit
				
### 'makeWebpage'
	make sure URL is not NULL
	‘normalize’ the URL
	check if its ‘internal’;
	any error from above return NULL
	if the hashtable parameter is defined,
		try to insert that URL into the hashtable of URLs seen
		if it was already in the table, return NULL
	
	make a new webpage for that URL, at depth+1
	use webpage fetch to retrieve a webpage for that URL,
	return NULL on any error from making or fetching
	return the webpage otherwise
### 'checkInput'
	check the number of args
	make sure the url is good by trying seed =  makeWebpage(seedURL, 0, NULL)
	check that the pageDirectory exists by making a hidden .crawler file in it
	make sure the depth argument is an integer greater than 0

## pagedir.c
### 'savepage'
	makes a new file with the ID as a name
	prints the URL of the webpage
	prints the Depth of the webpage
	prints the webpage html








