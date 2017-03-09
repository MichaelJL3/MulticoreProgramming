
#include "server.hpp"

Server::Server() : Socket(8000){
    bufferSize=DEFBUFFSIZE;
    listening=LISTEN;
    timeout.tv_sec=5;
	timeout.tv_usec=0;
    buffer=new char[bufferSize];
}

Server::Server(int size, int listeners, int port) : Socket(port){
    bufferSize=size;
    listening=listeners;
    timeout.tv_sec=5;
	timeout.tv_usec=0;
    buffer=new char[bufferSize];
}

Server::~Server(){
    delete buffer;
}

void Server::start(){
    int optval=1;
    socklen_t client_len;
    struct sockaddr_in client_addr;

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));

    if(bind(sockfd, (struct sockaddr *) &addr, sizeof(addr))<0)
        throw std::runtime_error("Failed Server Binding");
    
    if(listen(sockfd, listening)<0)
        throw std::runtime_error("Failed Server Listening");

    client_len = sizeof(client_addr);

    while(1){
        client_socket = accept(sockfd, (struct sockaddr *) &client_addr, &client_len);
        
	    if(setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout))<0) 
            throw std::runtime_error("Failed To Set Timeout");

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

    bytesRead=(bytesRead<0?0:bytesRead);
}

void Server::send(std::string msg){
    if(write(client_socket, msg.c_str(), msg.length())<0)
        throw std::runtime_error("Error Sending Message");
}

void Server::send(std::string msg, int conn){
    if(write(conn, msg.c_str(), msg.length())<0)
        throw std::runtime_error("Error Sending Message");
}

void Server::closeConn(){
    if(close(client_socket)<0)
        throw std::runtime_error("Error Closing Connection");
}

void Server::closeConn(int conn){
    if(close(conn)<0)
        throw std::runtime_error("Error Closing Connection");
}

int Server::getConn(){
    return client_socket;
}

int Server::getMsgSize(){
    return bytesRead;
}

char* Server::getMsg(){
    return buffer;
}