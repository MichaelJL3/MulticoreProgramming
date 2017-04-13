
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
LIB=-lpthread -Lhashing/cryptopp565/ -lcryptopp 
VRS=-std=c++11
FLAGS=$(WARNING) $(VRS)

PRGM=lab2
NAME=$(PRGM)

n=8
p=8000
sess=1000
reqs=100
addr=localhost

#compile flags
#all flags
ifdef A
FLAGS+= $(GDB) -DINFO -DSAFE -DSTATS
else
#debug flag
ifdef D
FLAGS+= $(GDB)
endif
#information flag
ifdef I
FLAGS+= -DINFO
endif
#safety flag
ifdef S
FLAGS+= -DSAFE
endif
#stats flag
ifdef ST
FLAGS+= -DSTATS
endif

endif

#lock free flag
ifdef LF
FLAGS+= -DLFREE
endif

#global lock flag
ifdef GL
FLAGS+= -DBIGLOCK
endif

#run flags
#turn on O for output piping to file
ifdef O
PIPE=> $(NAME)_runlog.txt
endif

#compile
c comp:
	$(COMPILER) $(FLAGS) -g -o $(NAME) network/*.cpp hashing/*.cpp httpreq/httpreq.cc httpreq/httpresp.cc threads/*.cpp $(PRGM).cpp $(LIB)

#run
r rn run:
	./$(PRGM) $(n) $(p) $(PIPE)

#create a test file
gen_test:
	python lab2_tools/lab2_workload.py -n=$(sess) -m=$(reqs) > requests.txt

#run httperf
http:
	httperf --verbose --server=$(addr) --port=$(p) --wsesslog $(sess),4,requests.txt

#analyze the run time stats
stats:
	python ./analyze/stats.py

#delete the binary
cl cln clean:
	rm $(NAME)
