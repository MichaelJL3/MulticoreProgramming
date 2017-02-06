
#lab0 - SET

##Language: C++

##MAKEFILE

compilation is extremely simple, there's a makefile with a number of
special flags and execution styles

for simple compilation and execution that meets the assignment standards 
	make comp > compiles default version uses vectors

	make exec > executes any compiled version
	
	make exec_pipe > executes and pipes output to file useful when 					 using the debug modes below 

there are also more complex modes 
	make dbg > compiles a version which prints information on fail

	make dbg_list > compiles a dbg version using lists instead of 					vectors
	
	make dbg_lazy > compiles a dbg version using a lazy test method 				explained below

	make comp_list > compiles a version using lists instead of 						 vectors
	
	make comp_lazy > compiles a version using the lazy test method 					 explained below

thoroughTest - inserts all the values in the set then checks that all the added values are within the set

lazyTest - inserts all the values in the set but check that the value
is in the set right after insertion, therefore errors in which new values somehow overwrite old ones would be missed 

##Files

lab0.cpp - holds the main function, the testing functions
hashmap.h - holds the templated set class used in lab0
makefile - used to compile and execute the code
readme.md