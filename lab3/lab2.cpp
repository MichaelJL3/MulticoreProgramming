
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
    int listen=1024;

    //accept arguments in terms of num threads and port num
    if(argc==4){
        threads=atoi(argv[1]);
        port=atoi(argv[2]);
        listen=atoi(argv[3]);
    }
    else if(argc==3){
        threads=atoi(argv[1]);
        port=atoi(argv[2]);
    }else if(argc>1){
        threads=atoi(argv[1]);
    }

    puts("Example Usage: ./server (num_threads) (port_num) (num_listeners)");
    printf("Starting Server Threads: %d\nListening On Port: %d\n", threads, port);

    //instantiate a server object and start
    ThreadPoolServer server(threads, port, listen);
    server.createThreads();
    server.start();

    //will never occur the server will infinite loop
    return 0;
}