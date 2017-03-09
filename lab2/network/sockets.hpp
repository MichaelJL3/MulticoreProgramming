
/*************************************\

 sockets.hpp
 Author: Michael Laucella
 Last Modified: 3/8/17

 create a basic tcp/ip socket

\*************************************/

#ifndef SOCKETS_HPP
#define SOCKETS_HPP

#include <arpa/inet.h>
#include <sstream>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <stdexcept>

#define DEFPORT 80 //default port number

class Socket{
protected:
    int sockfd, port;
    struct sockaddr_in addr;
public:
    Socket();       //constructor
    Socket(int p);  //constructor
    ~Socket();      //destructor
    void setup();   //setup the socket
};

#endif