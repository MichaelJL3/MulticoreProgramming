
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
dbg_ef:
	$(COMPILER) $(DEBUG) $(EFFIC) $(FLAGS) -o $(PRGM) $(PRGM).cpp $(LIB)

#compile a compare mode which gives stats of speedup between speedup and serial
compare stats:
	$(COMPILER) -DCOMPARE $(FLAGS) -o $(PRGM) $(PRGM).cpp $(LIB)

#compile a compare mode with efficiency
compare_ef:
	$(COMPILER) -DCOMPARE $(EFFIC) $(FLAGS) -o $(PRGM) $(PRGM).cpp $(LIB)

#compiles using debug efficiency and comparison
#not advised as the debug will cause more slowdown for the threads
everything every:
	$(COMPILER) -DCOMPARE $(DEBUG) $(EFFIC) $(FLAGS) -o $(PRGM) $(PRGM).cpp $(LIB)

#standard run
r rn run:
	./$(PRGM) $(n)

#run and pipe the output to a file
ex examine output out:
	./$(PRGM) $(n) > $(OUTPUT)

#delete the binary
cl cln clean:
	rm $(PRGM)