
#lab1 - Thread Safe Key Value Store

##Language: C++

##MAKEFILE

compilation is extremely simple, there's a makefile with a number of
special flags and execution styles

for simple compilation and execution that meets the assignment standards 
	make comp > compiles default version uses vectors
	make c

	make run > executes any compiled version
	make r

to specify number of threads use variable n
	make r n=4

if no number is specified a default of 4 is used as defined in lab1.h

to compile with more functionality view the makefile

there are also more complex modes 
	make dbg > compiles a version which prints informative messages
	make d

	make efficient > compiles a more efficient version using the method described below
	make effic
	make ef

	make dbg_ef > compiles a version with both efficient and debug flags

	make compare > compiles a version that compares the parallel to a sequential one

	make compare_ef > compiles a version that compares using the efficient version

	make everything > compiles a version that uses all the flags
	make every

##EFFICIENT MODE
	runs n-1 threads and has the main process also run the test

##DEBUG MODE
	uses a thread safe log to store messages received that print at the end of execution including crash points

##COMPARE MODE
	compares results with sequential version to see speedup and effeciency, the sequential code is entirely different so there are no synch overheads such as running parallel code with one thread

##Files

lab1.cpp  - holds the main function, the testing functions
lab1.h    - holds many macros, struct/functions definitions and includes
hashmap.h - holds the templated threadsafekeyvaluestore class
queue.h   - holds the templated threadsafelistenerqueue class
log.h 	  - holds a string log class used for the debug mode
makefile  - used to compile and execute the code
readme.md