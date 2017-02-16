
#####################################################
# Makefile											#
# Author: Michael Laucella							#
# Last Modified: 2/16/2017							#
#													#
# compiles and runs lab 1							#
#####################################################

COMPILER=g++
WARNING=-Wall
GDB=-g
LIB=-lpthread
VRS=-std=c++11
FLAGS=$(WARNING) $(VRS)
OUTPUT=runlog.txt
DEBUG=-DDEBUG $(GDB)
EFFIC=-DEFFICIENT

PRGM=lab1

#standard compile
c ++ comp:
	$(COMPILER) $(FLAGS) -o $(PRGM) $(PRGM).cpp $(LIB)

#turn on debug flags for informative messages upon certain errors
d dbg debug:
	$(COMPILER) $(DEBUG) $(FLAGS) -o $(PRGM) $(PRGM).cpp $(LIB)

#turn on efficiency allows the process to also run the function requiring one less thread
ef effic efficient:
	$(COMPILER) $(EFFIC) $(FLAGS) -o $(PRGM) $(PRGM).cpp $(LIB)

#turn on both efficiency and debugging
dbgef:
	$(COMPILER) $(DEBUG) $(EFFIC) $(FLAGS) -o $(PRGM) $(PRGM).cpp $(LIB)

#standard run
r rn run:
	./$(PRGM) $(n)

#run and pipe the output to a file
ex examine output out:
	./$(PRGM) $(n) > $(OUTPUT)

#delete the binary
cl cln clean:
	rm $(PRGM)