
#lab2 - Thread Pool Server

##Language: C++

##MAKEFILE

compilation is extremely simple, there's a makefile with a number of
special flags and execution styles

for simple compilation and execution that meets the assignment standards 
	make comp > compiles default version
	make c

	make run > executes any compiled version
	make r

to specify number of threads use variable n and port number with p
	make r n=4 p=80

if no number is specified a default of 4 is used as the number of threads
if no number is specified a default of 8000 is used as the port 

to compile with more functionality view the makefile and read the section below

there are also more complex modes 
	for compiling there are 4 Flags, D, I, S

	A: all uses all the flags
	D: debug flag compiles with debug mode just gdb flag
	I: information flag prints information req/res from the server
	S: catches non fatal run-time exceptions

	for running there is one flag O

	O: pipes the output of the run to a file

	to use these flags set them to one when running the command
	ex: make c D=1 E=1, make r O=1

##Files

lab2.cpp  - holds the main function, and starts the server
lab2.h    - holds inclusions

datastructs > holds threadsafe datastructures	
	hashmap.h - holds the templated threadsafekeyvaluestore class
	queue.h   - holds the templated threadsafelistenerqueue class

hashing >
	md5.hpp - md5 class performs md5 hashing functions
	md5.cpp - md5 class performs md5 hashing functions

network >
	server.hpp  - holds a server class for handling tcp/ip connections
	server.cpp  - holds a server class for handling tcp/ip connections
	sockets.hpp - holds a class for setting up tcp/ip connections
	sockets.cpp - holds a class for setting up tcp/ip connections

threads >
	threadPool.hpp 		 - holds a class for running multiple threads with access to class variables
	threadPool.cpp 		 - holds a class for running multiple threads with access to class variables
	threadPoolServer.hpp - holds a class for running a multithreaded server
	threadPoolServer.cpp - holds a class for running a multithreaded server

makefile  - used to compile and execute the code
readme.md

to run add the cryptopp library to the loader path
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/path_to_lab2/lab2/hashing/cryptopp565