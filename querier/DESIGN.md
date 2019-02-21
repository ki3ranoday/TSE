#Design for TSE 'Querier.c'

## main
check the argumentss
get user input
	break input into array of words and make all the words lowercase
	check input
		can't start or end with an 'and' or 'or' operator
		no non-alphabetic characters
	query the input
	clean up
	repeat

###Query
make a product accumulator and a sum accumulator
if the next word is not or, 
