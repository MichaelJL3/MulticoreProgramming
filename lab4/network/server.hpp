
#ifndef SERVER_HPP
#define SERVER_HPP

/*************************************\

 server.hpp
 Author: Michael Laucella
 Last Modified: 3/8/17

 create a tcp ip server skeleton

 **extends the socket class

\*************************************/

#ifdef INFO
#include <iostream>
#define LOG(...) std::cout << __VA_ARGS__ << std::endl
#endif

#include "sockets.hpp"

#define LISTEN 5
#define DEFBUFFSIZE 1024

class Server : public Socket{
    struct timeval timeout;
    char *buffer;
    int bytesRead;
    int bufferSize;
    int client_socket;
    int listening;
public:
    //constructors/destructors
    Server(int size, int listeners, int port);
    ~Server();
    void start();                           //start the server
    void recv();                            //recv a message from the server
    void send(std::string msg);             //send a message from the server to the last client
    void send(std::string msg, int conn);   //send a message from the server to a specific client
    void closeConn();                       //close the last client connection
    void closeConn(int conn);               //close a specific client connection
    int getConn();                          //get the last connection
    int getMsgSize();                       //get the last received message size
    char* getMsg();                         //get the last message
    virtual void handleConn();              //handle an incoming connection
};

#endif