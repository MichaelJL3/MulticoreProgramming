
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

#define DEFPORT 80

class Socket{
protected:
    int sockfd, port;
    struct sockaddr_in addr;
public:
    Socket();
    Socket(int p);
    ~Socket();
    void setup();
};

#endif