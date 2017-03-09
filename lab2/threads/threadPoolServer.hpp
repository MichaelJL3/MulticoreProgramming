
#ifndef THREAD_POOL_SERVER_HPP
#define THREAD_POOL_SERVER_HPP

#include "threadPool.hpp"
#include "../datastructs/queue.h"
#include "../datastructs/hashmap.h"
#include "../network/server.hpp"
#include "../httpreq/httpreq.hpp"
#include "../httpreq/httpresp.hpp"
#include "../hashing/md5.hpp"

struct Container{
    int conn;
    int msgSize;
    char* message;
public:
    Container(int c, char* msg, int ms){
        conn=c;
        message=msg;
        msgSize=ms;
    }
};

class ThreadPoolServer : public ThreadPool, public Server{
    ThreadSafeKVStore<std::string, std::string> hmap;
	ThreadSafeListenerQueue<Container*> queue;
public:
    ThreadPoolServer(int threads, int port);
    void* run();
    void handleConn();
};

#include "threadPoolServer.cpp" 

#endif