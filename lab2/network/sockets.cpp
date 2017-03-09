
/*************************************\

 sockets.cpp
 Author: Michael Laucella
 Last Modified: 3/8/17

 implement the tcp/ip soscket class
 in sockets.hpp

 **throws run-time exception 
   on any failure

\*************************************/

#include "sockets.hpp"

//constructor
Socket::Socket(){
    port=DEFPORT;
    setup();
}

//constructor
Socket::Socket(int p){
    port=p;
    setup();
}

//destructor
Socket::~Socket(){
    close(sockfd);
}

//setup the socket
void Socket::setup(){
    //setup the tcp/ip socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd<0) 
        throw std::runtime_error("Failed To Open Socket");

    //setup the address and port of the socket
    bzero((char *) &addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
}
