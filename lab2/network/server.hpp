
#ifndef SERVER_HPP
#define SERVER_HPP

#include "sockets.hpp"

#define LISTEN 5
#define DEFBUFFSIZE 1024

class Server : public Socket{
    char *buffer;
    int bytesRead;
    int bufferSize;
    int client_socket;
public:
    Server();
    Server(int size);
    ~Server();
    void start();
    void recv();
    void send(std::string msg);
    void closeConn();
    char* getMsg();
    int getMsgSize();
    virtual void handleConn();
};

#endif