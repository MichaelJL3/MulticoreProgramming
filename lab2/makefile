
#####################################################
# Makefile											#
# Author: Michael Laucella							#
# Last Modified: 3/2/2017							#
#													#
# compiles and runs lab 2							#
#####################################################

COMPILER=g++
WARNING=-Wall
GDB=-g
LIB=-lpthread
VRS=-std=c++11
FLAGS=$(WARNING) $(VRS)

PRGM=lab2
NAME=$(PRGM)

#compile flags
#turn on A for all flags
ifdef A
FLAGS+= -DDEBUG -DEFFICIENT -DCOMPARE
#if not all check for individual flags
else
#degug flag
ifdef D
FLAGS+= -DDEBUG
endif
#efficient mode flag
ifdef E
FLAGS+= -DEFFICIENT
endif
#comparison mode flag
ifdef C
FLAGS+= -DCOMPARE
endif
endif

#DEBUG add the g flag for GDB and prints out information from the
#run, will be slower requires more thread safe structures

#EFFICIENT has the main run an instance of the test therefore it
#runs n-1 threads and has less creation overhead

#COMPARE compares the result time from the threads with that of 
#a bare minimum sequential test

#run flags
#turn on O for output piping to file
ifdef O
PIPE=> $(NAME)_runlog.txt
endif

#compile
c ++ comp:
	$(COMPILER) $(FLAGS) -o $(NAME) $(PRGM).cpp $(LIB)

#run
r rn run:
	./$(PRGM) $(n) $(PIPE)

#delete the binary
cl cln clean:
	rm $(NAME)