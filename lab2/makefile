
#####################################################
# Makefile											#
# Author: Michael Laucella							#
# Last Modified: 3/8/2017							#
#													#
# compiles and runs lab 2							#
#####################################################

COMPILER=g++
WARNING=-Wall
GDB=-g
LIB=-lpthread -lcrypto++
VRS=-std=c++11
FLAGS=$(WARNING) $(VRS)

PRGM=lab2
NAME=$(PRGM)

#compile flags
#degug flag
ifdef D
FLAGS+= GDB
endif

#run flags
#turn on O for output piping to file
ifdef O
PIPE=> $(NAME)_runlog.txt
endif

#compile
c ++ comp:
	$(COMPILER) $(FLAGS) -o $(NAME) network/*.cpp hashing/*.cpp httpreq/httpreq.cc httpreq/httpresp.cc threads/*.cpp $(PRGM).cpp $(LIB)

#run
r rn run:
	./$(PRGM) $(n) $(p) $(PIPE)

#delete the binary
cl cln clean:
	rm $(NAME)