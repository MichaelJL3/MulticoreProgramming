
/*************************************\

 server.cpp
 Author: Michael Laucella
 Last Modified: 3/8/17

 implementation of the Server
 class in server.hpp

 **extends the socket class

 **not thread safe

 **throws run-time exception 
   on any failure

\*************************************/

#include "server.hpp"

//server constructor
Server::Server() : Socket(8000){
    bufferSize=DEFBUFFSIZE;
    listening=LISTEN;
    timeout.tv_sec=5;
	timeout.tv_usec=0;
    buffer=new char[bufferSize];
}

//server constructor
Server::Server(int size, int listeners, int port) : Socket(port){
    bufferSize=size;
    listening=listeners;
    timeout.tv_sec=5;
	timeout.tv_usec=0;
    buffer=new char[bufferSize];
}

//delete the message buffer
Server::~Server(){
    delete buffer;
}

//start the server
void Server::start(){
    int optval=1;
    socklen_t client_len;
    struct sockaddr_in client_addr;

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));

    #ifdef INFO 
    LOG("Starting Server on Port: " << port << "\nBinding:");
    #endif

    //bind the server
    if(bind(sockfd, (struct sockaddr *) &addr, sizeof(addr))<0)
        throw std::runtime_error("Failed Server Binding");

    //set the amount of active listeners in the queue
    if(listen(sockfd, listening)<0)
        throw std::runtime_error("Failed Server Listening");

    client_len = sizeof(client_addr);

    #ifdef INFO 
    LOG("Listening: " << listening);
    #endif

    //infinite loop
    while(1){
        //wait for incoming client connection
        client_socket = accept(sockfd, (struct sockaddr *) &client_addr, &client_len);
        
        #ifdef INFO
        LOG("\nClient Accepted: " << client_socket);
        #endif

        //set client timeout
	    //if(setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout))<0){
        //    #ifndef SAFE
        //    throw std::runtime_error("Failed To Set Timeout");
        //    #endif
        //}

        if (client_socket < 0){
            #ifndef SAFE
            throw std::runtime_error("Error Accepting Connection");
            #endif
        }

        //handle incoming connection
        handleConn();
    }
}

//overwrittable connection handler currently just serves as an echo server
void Server::handleConn(){
    recv();
    printf("Received Message:\n%s\n", buffer);
    send("Message Received\n");
    closeConn();
}

//receive a message from a client (timesout)
void Server::recv(){
    bzero(buffer, bufferSize);
    bytesRead=read(client_socket, buffer, bufferSize-1);

    //size of message cannot be less then 0
    bytesRead=(bytesRead<0?0:bytesRead);
}

//send a message to the last client
void Server::send(std::string msg){
    if(write(client_socket, msg.c_str(), msg.length())<0){
        #ifndef SAFE
        throw std::runtime_error("Error Sending Message");
        #endif
    }
}

//send a message to a client
void Server::send(std::string msg, int conn){
    if(write(conn, msg.c_str(), msg.length())<0){
        #ifndef SAFE
        throw std::runtime_error("Error Sending Message");
        #endif
    }
}

//close the last client connection
void Server::closeConn(){
    if(close(client_socket)<0){
        #ifndef SAFE
        throw std::runtime_error("Error Closing Connection");
        #endif
    }
}

//close a client connection
void Server::closeConn(int conn){
    if(close(conn)<0){
        #ifdef SAFE
        throw std::runtime_error("Error Closing Connection");
        #endif
    }
}

//get the client connection
int Server::getConn(){
    return client_socket;
}

//get the message size
int Server::getMsgSize(){
    return bytesRead;
}

//get the message buffer
char* Server::getMsg(){
    return buffer;
}