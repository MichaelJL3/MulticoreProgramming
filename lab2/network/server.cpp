
#include "server.hpp"

Server::Server() : Socket(8000){
    bufferSize=DEFBUFFSIZE;
    buffer=new char[bufferSize];
}

Server::Server(int size) : Socket(8000){
    bufferSize=size;
    buffer=new char[bufferSize];
}

Server::~Server(){
    delete buffer;
}

void Server::start(){
    int optval = 1;
    socklen_t client_len;
    struct sockaddr_in client_addr;

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));

    if(bind(sockfd, (struct sockaddr *) &addr, sizeof(addr))<0)
        throw std::runtime_error("Failed Server Binding");
    
    if(listen(sockfd, LISTEN)<0)
        throw std::runtime_error("Failed Server Listening");

    client_len = sizeof(client_addr);

    while(1){
        client_socket = accept(sockfd, (struct sockaddr *) &client_addr, &client_len);
     
        if (client_socket < 0) 
            throw std::runtime_error("Error Accepting Connection");

        handleConn();
    }
}

void Server::handleConn(){
    recv();
    printf("Received Message:\n%s\n", buffer);
    send("Message Received\n");
    closeConn();
}

void Server::recv(){
    bzero(buffer, bufferSize);
    bytesRead=read(client_socket, buffer, bufferSize-1);

    if(bytesRead<0)
        throw std::runtime_error("Error Receiving Message");
}

void Server::send(std::string msg){
    if(write(client_socket, msg.c_str(), msg.length())<0)
        throw std::runtime_error("Error Sending Message");
}

void Server::closeConn(){
    close(client_socket);
}

char* Server::getMsg(){
    return buffer;
}

int Server::getMsgSize(){
    return bytesRead;
}