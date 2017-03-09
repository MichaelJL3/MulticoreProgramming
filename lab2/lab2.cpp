
#include "lab2.h"

/*************************************\

 lab2.cpp
 Author: Michael Laucella
 Last Modified: 3/8/17

 runs a thread safe kv server store 
 among n threads using a thread pool
 and MD5 key hashing

 failure on a server function from setup
 to accept will cause a crash with the 
 exception of read due to timeout 
 functionality

 compile with info mode to get detailed
 request information about requests
 and responses

 compile with safe mode to catch non 
 critical errors and prevent crashing

\*************************************/

int main(int argc, char** argv){
    //default values
    int port=8000;
    int threads=4;

    //accept arguments in terms of num threads and port num
    if(argc==3){
        threads=atoi(argv[1]);
        port=atoi(argv[2]);
    }else if(argc>1){
        threads=atoi(argv[1]);
    }

    //instantiate a server object and start
    ThreadPoolServer server(threads, port);
    server.createThreads();
    server.start();

    //will never occur the server will infinite loop
    return 0;
}