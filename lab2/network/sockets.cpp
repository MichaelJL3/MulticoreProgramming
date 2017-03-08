
#include "sockets.hpp"

Socket::Socket(){
    port=DEFPORT;
    setup();
}

Socket::Socket(int p){
    port=p;
    setup();
}

Socket::~Socket(){
    close(sockfd);
}

void Socket::setup(){
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd<0) 
        throw std::runtime_error("Failed To Open Socket");

    bzero((char *) &addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
}
