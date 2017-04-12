
#ifndef THREAD_POOL_SERVER_HPP
#define THREAD_POOL_SERVER_HPP

/*************************************\

 threadPoolServer.hpp
 Author: Michael Laucella
 Last Modified: 3/8/17

 skeleton for mulithreaded tcp/ip server

 extends both the threadpool and the Server
 class

 used for a very specific purpose in 
 creating a kv server store

 uses crypto++ for md5 key hashing
 and provided httpreq/httpresp for
 http parsing

 uses a queue listener for message
 passing between threadpool and 
 server

\*************************************/

#include "threadPool.hpp"
#include "../datastructs/queue.h"
#include "../datastructs/hashmap.h"
#include "../network/server.hpp"
#include "../httpreq/httpreq.hpp"
#include "../httpreq/httpresp.hpp"
#include "../hashing/md5.hpp"

#ifdef STATS
#include <mutex>
#include <ctime>
#include <fstream>
#include <string>
#endif

class ThreadPoolServer : public ThreadPool, public Server{
    ThreadSafeKVStore<std::string, std::string> hmap;
    #ifndef LFREE
	ThreadSafeListenerQueue<int> queue;
    #else
    LockFreeQueue<int> queue;
    #endif
    #ifdef STATS
    std::mutex timeLock;
    std::ofstream fd;
    #endif
public:
    ThreadPoolServer(int threads, int port, int listen);    //constructor
    void* run();         //thread run function
    void handleConn();   //connection handling
};

#endif