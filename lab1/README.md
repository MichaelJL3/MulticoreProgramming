
#lab1 - Thread Safe Key Value Store

##Language: C++

##MAKEFILE

compilation is extremely simple, there's a makefile with a number of
special flags and execution styles

for simple compilation and execution that meets the assignment standards 
	make comp > compiles default version
	make c

	make run > executes any compiled version
	make r

to specify number of threads use variable n
	make r n=4

if no number is specified a default of 4 is used as defined in lab1.h

to compile with more functionality view the makefile and read the section below

there are also more complex modes 
	for compiling there are 4 flags D, E, C, A

	A: all uses all the flags
	D: debug flag compiles with debug mode
	C: comparison compiles with comparison mode
	E: efficiency compiles with efficiency mode

	for running there is one flag O

	O: pipes the output of the run to a file

	to use these flags set them to one when running the command
	ex: make c D=1 E=1, make r O=1

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