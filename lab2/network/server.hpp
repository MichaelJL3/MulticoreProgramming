
#ifndef SERVER_HPP
#define SERVER_HPP

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
    Server();
    Server(int size, int listeners, int port);
    ~Server();
    void start();
    void recv();
    void send(std::string msg);
    void send(std::string msg, int conn);
    void closeConn();
    void closeConn(int conn);
    int getConn();
    int getMsgSize();
    char* getMsg();
    virtual void handleConn();
};

#endif