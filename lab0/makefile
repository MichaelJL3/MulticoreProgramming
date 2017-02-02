
#custom fields
std=-std=c++11
info=INFORM
list=USING_LIST
lazy=LAZY

#debug mode only prints extra messages on failures
dbg:
	g++ $(std) -D$(info) -o lab0 lab0.cpp

#same as dbg but uses lists instead of vectors
dbg_list:
	g++ $(std) -D$(list) -D$(info) -o lab0 lab0.cpp

#same as dbg but runs a lazier test instead of the default thorough test
dbg_lazy:
	g++ $(std) -D$(lazy) -D$(info) -o lab0 lab0.cpp

#compile the standard default code
comp:
	g++ $(std) -o lab0 lab0.cpp

#compile using lists instead of vectors
comp_list:
	g++ $(std) -D$(list) -o lab0 lab0.cpp

#compiles with default but runs a lazier test instead of the default thorough test
comp_lazy:
	g++ $(std) -D$(lazy) -o lab0 lab0.cpp

#run the exe
exec:
	./lab0

#run the exe but pipe the output to a file
exec_pipe:
	./lab0 > output.txt
