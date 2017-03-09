
#include "lab2.h"

int main(int argc, char** argv){
    int port=8000;
    int threads=4;

    if(argc==3){
        threads=atoi(argv[1]);
        port=atoi(argv[2]);
    }else if(argc>1){
        threads=atoi(argv[1]);
    }

    ThreadPoolServer server(threads, port);
    server.createThreads();
    server.start();

    return 0;
}