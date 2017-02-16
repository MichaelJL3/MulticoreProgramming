
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

c ++ comp:
	$(COMPILER) $(FLAGS) -o $(PRGM) $(PRGM).cpp $(LIB)

d dbg debug:
	$(COMPILER) $(DEBUG) $(FLAGS) -o $(PRGM) $(PRGM).cpp $(LIB)

ef effic efficient:
	$(COMPILER) $(EFFIC) $(FLAGS) -o $(PRGM) $(PRGM).cpp $(LIB)

r rn run:
	./$(PRGM)

ex examine output out:
	./$(PRGM) > $(OUTPUT)

cl cln clean:
	rm $(PRGM)