
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

PRGM=lab4
NAME=$(PRGM)

CRNCH3=172.22.49.57

n=8
p=8000
sess=1000
reqs=100
addr=localhost
threads=1 2 4 8 16 32

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
endif

#lock free flag
ifdef LF
FLAGS+= -DLFREE
endif

#global lock flag
ifdef GL
FLAGS+= -DBIGLOCK
endif

#no cache flag
ifdef NC
FLAGS+= -DNO_CACHE
endif

#cache list
ifdef CL
FLAGS+= -DCACHE_LIST
endif

#run flags
#turn on O for output piping to file
ifdef O
PIPE=> $(NAME)_runlog.txt
endif

#compile
c comp:
	$(COMPILER) $(FLAGS) -g -o $(NAME) files/*.cpp network/*.cpp hashing/*.cpp httpreq/httpreq.cc httpreq/httpresp.cc threads/*.cpp $(PRGM).cpp $(LIB)

#run
r rn run:
	./$(PRGM) $(n) $(p) $(PIPE)

#create a test file
gen_test:
	python lab2_tools/lab2_workload.py -n=$(sess) -m=$(reqs) -z > requests.txt

#run httperf
http:
	httperf --verbose --server=$(addr) --port=$(p) --wsesslog $(sess),4,requests.txt

#analyze the run time stats
stats:
	python ./analyze/stats.py

#delete the binary
cl cln clean:
	rm $(NAME)
